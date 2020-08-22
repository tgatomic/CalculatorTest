#include <string>


#include "Command.h"


bool Command::IsDigit(std::string str)
{
	char* p;
	long int convertedNumber = std::strtol(str.c_str(), &p, 10);

	if (*p)
	{
		return false;
	}
	return true;
}


Command::Command(std::string reg, std::string command, std::string number)
{

	this->Register = reg;
	this->Operation = CommandType.find(command)->second;
	
	if (IsDigit(number))
	{
		this->Value = std::stoi(number);
	}
	else
	{
		this->Value = 0;
	}
}