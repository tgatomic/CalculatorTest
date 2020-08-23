#pragma once
#include <vector>
#include "Console.h"

class Calculator
{
private:
	typedef int (*funptr)(int, int);
	enum CommandTypeEnum { ADDITION, SUBTRACT, MULTIPLY, DIVIDE, RESULT, PRINT, QUIT, ERROR };
	std::map<std::string, CommandTypeEnum> CommandType = {
		{"add", ADDITION},
		{"subtract", SUBTRACT},
		{"multiply", MULTIPLY},
		{"divide", DIVIDE},
		{"result", RESULT},
		{"print", PRINT},
		{"quit", QUIT},
		{"error", ERROR},
	};
	
	std::thread incommingCalculationsThread;
	Console *consoleref;
	std::map<std::string, Command> Registers;

	void HandleIncommingCalculations();
	void PrintToConsole(std::string);
	void CalculateResult(Command, funptr[]);



public:
	std::atomic<bool> RunThread = true;

	Calculator(Console* console);
	~Calculator();
};

