#include <thread>

#include "Calculator.h"
#include "Command.h"

int Add(int a, int b) {
	return a + b;
}

int Subtract(int a, int b) {
	return a - b;
}

int Multiply(int a, int b) {
	return a * b;
}

int Divide(int a, int b) {
	return a / b;
}


Calculator::Calculator(Console* console)
{
	this->consoleref = console;

	incommingCalculationsThread = std::thread(&Calculator::HandleIncommingCalculations, this);
}

Calculator::~Calculator()
{
	this->incommingCalculationsThread.join();
}

void Calculator::HandleIncommingCalculations()
{
	funptr operations[] = { Add, Subtract, Multiply, Divide };

	while (this->RunThread)
	{
		consoleref->CommandQueueMutex.lock();
		if (!consoleref->CommandQueue.empty())
		{
			Command newCommandString = consoleref->CommandQueue.front();

			// To prevent clearing the queue of this event before main has time to read it
			if (newCommandString.Operation == "quit")
			{
				this->PrintToConsole("quit");
				consoleref->CommandQueueMutex.unlock();
				return;
			}

			consoleref->CommandQueue.pop();

			// Figure our what type of command we recieved
			CommandTypeEnum command = CommandType.find(newCommandString.Operation)->second;			
			
			switch (command)
			{
			case PRINT:
				this->PrintToConsole(std::to_string(Registers[newCommandString.Register].Value));
				break;
			case RESULT:
				this->CalculateResult(newCommandString, operations);
				this->PrintToConsole(std::to_string(Registers[newCommandString.Register].Value));
				break;
			case DIVIDE:
				if (newCommandString.Value == 0)
				{
					this->PrintToConsole("Division by zero not allowed");
				}
				else
				{
					this->CalculateResult(newCommandString, operations);
				}
				break;
			case ADDITION: case SUBTRACT: case MULTIPLY:
				this->CalculateResult(newCommandString, operations);
				break;

			default:
				this->PrintToConsole("Error for register: " + newCommandString.Register);
			}
		}
		consoleref->CommandQueueMutex.unlock();
	}
}

void Calculator::CalculateResult(Command command, funptr operations[])
{
	if (Registers.find(command.Register) != Registers.end())
	{
		// Have an array with functions that corespond to the operations and use it to get the result,
		// similar as can be found here: https://stackoverflow.com/questions/52900557/can-you-store-arithmetic-operators-in-an-array-i-e-in-c
		// So the code ould look like

		// int result = (operations[newCommandString.Operation])(Registers[newCommandString.Register].Value, newCommandString.Value)
		// Registers[newCommandString.Register].Value = result;

		// Get what operations is supposed to be done; add, subtract, divide, multiply
		CommandTypeEnum arethmicOperation = CommandType.find(command.Operation)->second;

		int result = 0;
		// Do the operation using functionpointer
		if (command.SourceRegister != "")
		{
			if (Registers.find(command.SourceRegister) != Registers.end())
			{
				result = (operations[arethmicOperation])(Registers[command.Register].Value, Registers[command.SourceRegister].Value);
			}
			else
			{
				this->PrintToConsole("Error: Coudln't find register " + command.SourceRegister + " in register list");
				return;
			}
		}
		else
		{
			result = (operations[arethmicOperation])(Registers[command.Register].Value, command.Value);
		}

		// Save the result
		Registers[command.Register].Value = result;
		
		// Add the "lazy evaluation"-code
		if (command.TargetRegister != "")
		{
			CommandTypeEnum arethmicOperationOriginal = CommandType.find(command.OriginalOperation)->second;
			Registers[command.TargetRegister].Value = (operations[arethmicOperationOriginal])(Registers[command.Register].Value, command.Value);
		}

	}
	else
	{
		Registers.insert({ command.Register, command });
	}
}

void Calculator::PrintToConsole(std::string message)
{
	consoleref->txMessageMutex.lock();
	consoleref->txMessageQueue.push(message);
	consoleref->txMessageMutex.unlock();
}

