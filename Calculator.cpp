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
	funptr arethmicFunctions[] = { Add, Subtract, Multiply, Divide };

	while (this->RunThread)
	{
		consoleref->CommandQueueMutex.lock();
		if (!consoleref->CommandQueue.empty())
		{
			Command newCommand = consoleref->CommandQueue.front();

			// To prevent clearing the queue of this event before main has time to read it and stop the program
			if (newCommand.Operation == "quit")
			{
				this->PrintToConsole("quit");
				consoleref->CommandQueueMutex.unlock();
				return;
			}

			consoleref->CommandQueue.pop();

			// Make sure the operation is valid before continuing
			if (CommandType.find(newCommand.Operation) == CommandType.end())
			{
				this->PrintToConsole("Operation " + newCommand.Operation + " not found. Input not added to register.");
				consoleref->CommandQueueMutex.unlock();
				continue;
			}

			CommandTypeEnum command = CommandType.find(newCommand.Operation)->second;			
			
			switch (command)
			{
			case PRINT:
				this->PrintRegisterToConsole(newCommand, arethmicFunctions);
				break;

			case RESULT:
				this->ProcessRegisterChange(newCommand, arethmicFunctions);
				this->PrintToConsole(std::to_string(Registers[newCommand.Register].Value));
				break;

			case DIVIDE:
				if (newCommand.Value == 0)
				{
					this->PrintToConsole("Division by zero not allowed");
				}
				else
				{
					this->ProcessRegisterChange(newCommand, arethmicFunctions);
				}
				break;

			case ADDITION: case SUBTRACT: case MULTIPLY:
				this->ProcessRegisterChange(newCommand, arethmicFunctions);
				break;

			default:
				this->PrintToConsole("Error in register: " + newCommand.Register);
			}
		}
		consoleref->CommandQueueMutex.unlock();
	}
}

void Calculator::ProcessRegisterChange(Command command, funptr arethmicFunctions[])
{
	if (Registers.find(command.Register) != Registers.end())
	{
		if (!command.IsNumber)
		{
			Registers[command.Register].LinkedRegisters.push_back(command);
			return;
		}

		int result = 0;
		CommandTypeEnum arethmicOperation = CommandType.find(command.Operation)->second;

		if ( command.SourceRegister != "")
		{
			// The new input contains a register name as value New command is a register
			result = (arethmicFunctions[arethmicOperation])(Registers[command.Register].Value, Registers[command.SourceRegister].Value);
			Registers[command.Register].Value = result;
		}
		else
		{
			// New input value is an int
			result = (arethmicFunctions[arethmicOperation])(Registers[command.Register].Value, command.Value);
			Registers[command.Register].Value = result;
		}
	}
	else
	{
		if (!command.IsNumber)
		{
			// If the new input is a register, add it to the first registers, linked list
			Registers.insert({ command.Register, command });
			Registers[command.Register].LinkedRegisters.push_back(command);
		}
		else
		{
			// If the new input is a number, add it to the map with the number as value
			Registers.insert({ command.Register, command });
		}
	}
}

int Calculator::SolveLinkedRegisters(std::string registerName, funptr arethmicFunctions[])
{
	std::list<Command>::iterator it;
	int result = 0;

	for (it = Registers[registerName].LinkedRegisters.begin(); it != Registers[registerName].LinkedRegisters.end(); ++it) {

		if (Registers[it->SourceRegister].LinkedRegisters.size() > 0)
		{
			int otherRegister = SolveLinkedRegisters(it->SourceRegister, arethmicFunctions);
		}

		CommandTypeEnum arethmicOperation = CommandType.find(it->OriginalOperation)->second;
		result = (arethmicFunctions[arethmicOperation])(result, Registers[it->SourceRegister].Value);
	}

	return result;
}

void Calculator::PrintLinkedRegister(Command command, funptr arethmicFunctions[])
{
	std::list<Command>::iterator it;
	int result = 0;
	int otherRegister = 0;

	for (it = Registers[command.Register].LinkedRegisters.begin(); it != Registers[command.Register].LinkedRegisters.end(); ++it) {

		if (Registers[it->SourceRegister].LinkedRegisters.size() > 0)
		{
			otherRegister = SolveLinkedRegisters(it->SourceRegister, arethmicFunctions);
		}
		
		CommandTypeEnum arethmicOperation = CommandType.find(it->OriginalOperation)->second;
		result = (arethmicFunctions[arethmicOperation])(result, Registers[it->SourceRegister].Value + otherRegister);
	}

	this->PrintToConsole(std::to_string(result));
}

void Calculator::PrintRegisterToConsole(Command command, funptr arethmicFunctions[])
{
	if (Registers.find(command.Register) != Registers.end())
	{
		if (Registers[command.Register].LinkedRegisters.size() > 0)
		{
			PrintLinkedRegister(command, arethmicFunctions);
		}
		else
		{
			this->PrintToConsole(std::to_string(Registers[command.Register].Value));
		}
	}
	else
	{
		this->PrintToConsole("Register " + command.Register + " not found");
	}
	
}

void Calculator::PrintToConsole(std::string message)
{
	consoleref->txMessageMutex.lock();
	consoleref->txMessageQueue.push(message);
	consoleref->txMessageMutex.unlock();
}

