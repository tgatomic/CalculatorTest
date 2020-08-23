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


Command::Command(std::string reg, std::string command, std::string value)
{
	this->Register = reg;
	this->Operation = command;
	this->TargetRegister = "";
	this->OriginalOperation = "";

	if (value == "")
	{
		this->Value = 0;
		this->SourceRegister = "";
		this->IsNumber = false;
	}
	else if (IsDigit(value))
	{
		this->Value = std::stoi(value);
		this->SourceRegister = "";
		this->IsNumber = true;
	}
	else
	{
		this->Value = 0;
		this->SourceRegister = value;
		this->TargetRegister = value;
		this->IsNumber = false;
		this->OriginalOperation = command;
	}
}