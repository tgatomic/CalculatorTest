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
				if (Registers[newCommandString.Register].linkedRegisters.size() > 0)
				{
					PrintLinkedRegister(newCommandString, operations);
				}
				else
				{
					this->PrintToConsole(std::to_string(Registers[newCommandString.Register].Value));
				}
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
		if (!command.IsNumber)
		{
			Registers[command.Register].linkedRegisters.push_back(command);
			return;
		}

		int result = 0;
		CommandTypeEnum arethmicOperation = CommandType.find(command.Operation)->second;

		// New command is a register
		if ( command.SourceRegister != "")
		{
			result = (operations[arethmicOperation])(Registers[command.Register].Value, Registers[command.SourceRegister].Value);
			Registers[command.Register].Value = result;
		}
		// New command is an int
		else
		{
			result = (operations[arethmicOperation])(Registers[command.Register].Value, command.Value);
			Registers[command.Register].Value = result;
		}
	}
	else
	{
		if (!command.IsNumber)
		{
			Registers.insert({ command.Register, command });
			Registers[command.Register].linkedRegisters.push_back(command);
		}
		else
		{
			Registers.insert({ command.Register, command });
		}
	}
}

int Calculator::SolveLinkedRegisters(std::string registerName, funptr operations[])
{
	std::list<Command>::iterator it;
	int result = 0;

	for (it = Registers[registerName].linkedRegisters.begin(); it != Registers[registerName].linkedRegisters.end(); ++it) {

		if (Registers[it->SourceRegister].linkedRegisters.size() > 0)
		{
			int otherRegister = SolveLinkedRegisters(it->SourceRegister, operations);
		}

		CommandTypeEnum arethmicOperation = CommandType.find(it->OriginalOperation)->second;
		result = (operations[arethmicOperation])(result, Registers[it->SourceRegister].Value);
	}

	return result;
}

void Calculator::PrintLinkedRegister(Command command, funptr operations[])
{
	std::list<Command>::iterator it;
	int result = 0;
	int otherRegister = 0;

	for (it = Registers[command.Register].linkedRegisters.begin(); it != Registers[command.Register].linkedRegisters.end(); ++it) {

		if (Registers[it->SourceRegister].linkedRegisters.size() > 0)
		{
			otherRegister = SolveLinkedRegisters(it->SourceRegister, operations);
		}
		
		CommandTypeEnum arethmicOperation = CommandType.find(it->OriginalOperation)->second;
		result = (operations[arethmicOperation])(result, Registers[it->SourceRegister].Value + otherRegister);
	}

	this->PrintToConsole(std::to_string(result));
}

void Calculator::PrintToConsole(std::string message)
{
	consoleref->txMessageMutex.lock();
	consoleref->txMessageQueue.push(message);
	consoleref->txMessageMutex.unlock();
}

