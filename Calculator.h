#pragma once
#include <vector>
#include "Console.h"
#include <string>

class Calculator
{
private:
	typedef int (*funptr)(int, int);
	enum CommandTypeEnum { ADDITION, SUBTRACT, MULTIPLY, DIVIDE, RESULT, PRINT, QUIT, ERROR };
	
	// With this, it's possibel to add several operation/commands and map them in the program. 
	// to for example allow shorthand names on addition, but still keep the code clear. 
	std::map<std::string, CommandTypeEnum> CommandType = {
		{"add", ADDITION},
		{"addition", ADDITION},
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

	void HandleIncommingCalculations();
	void PrintLinkedRegister(Command, funptr[]);
	int SolveLinkedRegisters(std::string, funptr[]);
	void PrintToConsole(std::string);
	void PrintRegisterToConsole(Command, funptr[]);

public:
	std::atomic<bool> RunThread = true;
	std::map<std::string, Command> Registers;

	void ProcessRegisterChange(Command, funptr[]);
	Calculator(Console* console);
	~Calculator();
};