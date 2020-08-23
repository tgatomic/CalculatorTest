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
	this->RunThread = false;
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

			case ADD: case SUBTRACT: case MULTIPLY: case DIVIDE:
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

		// Do the operation using functionpointer
		int result = (operations[arethmicOperation])(Registers[command.Register].Value, command.Value);

		// Save the result
		Registers[command.Register].Value = result;
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

