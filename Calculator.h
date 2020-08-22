#pragma once
#include "Console.h"

class Calculator
{
private:
	std::thread incommingCalculationsThread;
	Console *consoleref;

	void HandleIncommingCalculations();
	void PrintToConsole(std::string);

public:
	std::atomic<bool> RunThread = true;

	Calculator(Console* console);
	~Calculator();
};

