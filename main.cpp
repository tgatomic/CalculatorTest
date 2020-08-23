// main.c : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <thread>

#include "Console.h"
#include "Calculator.h"

int main()
{
    std::cout << "Hello World!\n";

    Console console;
    Calculator calculator(&console);
	bool RunMain = true;

    while (RunMain)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

		console.CommandQueueMutex.lock();
		if (!console.CommandQueue.empty())
		{
			Command newCommandString = console.CommandQueue.front();

			if (newCommandString.Operation.compare("quit") == 0)
			{
				RunMain = false;
			}
		}
		console.CommandQueueMutex.unlock();
    }

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
