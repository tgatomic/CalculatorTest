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
	this->listenerThread.join();
	this->parseThread.join();

	std::cout << "Threads quit";
}

void Console::GetUserInput()
{
	std::cout << "Starting Thread\n";
	std::string command;
	char nextCharacter;

	while (RunThread)
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
		}
		else
		{
			command += nextCharacter;
		}
	}
}

void Console::ParseIncommingMessages()
{
	while (RunThread)
	{
		rxMessageMutex.lock();
		if (!rxMessageQueue.empty())
		{
			std::string newCommandString = rxMessageQueue.front();
			rxMessageQueue.pop();

			// CHECK IF QUIT IS WRITTEN AND EXIT APPLICATION

			this->CommandQueueMutex.lock();
			this->CommandQueue.push(CommandHandler::GetCommand(newCommandString));
			this->CommandQueueMutex.unlock();
		}
		rxMessageMutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Console::WriteOutgoingMessages()
{
	while (true)
	{
		txMessageMutex.lock();
		if (!txMessageQueue.empty())
		{
			std::string message = this->txMessageQueue.front();
			this->txMessageQueue.pop();

			this->Write(message);
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