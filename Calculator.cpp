#include <thread>

#include "Calculator.h"
#include "Command.h"

Calculator::Calculator(Console* console)
{
	this->consoleref = console;

	incommingCalculationsThread = std::thread(&Calculator::HandleIncommingCalculations, this);
}

Calculator::~Calculator()
{
	this->RunThread = false;
}

void Calculator::HandleIncommingCalculations()
{
	while (this->RunThread)
	{
		consoleref->CommandQueueMutex.lock();
		if (!consoleref->CommandQueue.empty())
		{
			Command newCommandString = consoleref->CommandQueue.front();
			consoleref->CommandQueue.pop();

			this->PrintToConsole("New calculation recieved");
		}
		consoleref->CommandQueueMutex.unlock();
	}
}

void Calculator::PrintToConsole(std::string message)
{
	consoleref->txMessageMutex.lock();
	consoleref->txMessageQueue.push(message);
	consoleref->txMessageMutex.unlock();
}