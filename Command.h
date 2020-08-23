#pragma once
#include <string>
#include <map>

class Command
{
private:
	bool IsDigit(std::string str);


public:
	int Value;
	std::string Register;
	std::string SourceRegister;
	std::string Error;
	std::string Operation;
	bool IsNumber;

	Command(std::string, std::string, std::string);
	Command() {};
};

