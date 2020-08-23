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

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
