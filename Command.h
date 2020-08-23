#pragma once
#include <string>
#include <map>
#include <list>

class Command
{
private:
	bool IsDigit(std::string str);


public:
	int Value;
	std::string Register;
	std::string SourceRegister;
	std::string TargetRegister;
	std::string OriginalOperation;
	std::string Error;
	std::string Operation;
	bool IsNumber;
	std::list <Command> linkedRegisters;

	Command(std::string, std::string, std::string);
	Command() {};
};

