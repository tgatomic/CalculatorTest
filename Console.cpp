#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <queue>

#include "Console.h"
#include "CommandHandler.h"

std::mutex io_mutex;

Console::Console()
{
	listenerThread = std::thread(&Console::GetUserInput, this);
	parseThread = std::thread(&Console::ParseIncommingMessages, this);
	writeThread = std::thread(&Console::WriteOutgoingMessages, this);
}

Console::~Console()
{

}

void Console::GetUserInput()
{
	std::cout << "Type <registername> <operation> <register or number>\n";
	std::string command;
	char nextCharacter;

	while (this->RunThread)
	{
		while (std::cin.peek() == EOF && RunThread)
		{
			std::this_thread::yield();
		}

		io_mutex.lock();
		nextCharacter = std::cin.get();
		io_mutex.unlock();

		if (nextCharacter == '\n')
		{
			rxMessageMutex.lock();
			rxMessageQueue.push(command);
			rxMessageMutex.unlock();
			command = "";

			if (command.compare("quit") == 0)
			{
				this->RunThread = false;
			}
		}
		else
		{
			command += nextCharacter;
		}
	}
}

void Console::ParseIncommingMessages()
{
	while (this->RunThread)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		rxMessageMutex.lock();
		if (!rxMessageQueue.empty())
		{
			std::string newCommandString = rxMessageQueue.front();
			rxMessageQueue.pop();

			this->CommandQueueMutex.lock();
			Command newCommand = CommandHandler::GetCommand(newCommandString);
			this->CommandQueue.push(newCommand);
			this->CommandQueueMutex.unlock();

			if (newCommand.Operation.compare("quit") == 0)
			{
				this->RunThread = false;
			}
		}
		rxMessageMutex.unlock();
	}
}

void Console::WriteOutgoingMessages()
{
	while (RunThread)
	{
		txMessageMutex.lock();
		if (!txMessageQueue.empty())
		{
			std::string message = this->txMessageQueue.front();
			this->txMessageQueue.pop();

			if (message.compare("quit") == 0)
			{
				this->RunThread = false;
			}
			else 
			{
				this->Write(message);
			}
		}
		txMessageMutex.unlock();
	}
}

void Console::Write(std::string message)
{
	io_mutex.lock();
	std::cout << message << std::endl;
	io_mutex.unlock();
}