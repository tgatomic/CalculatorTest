#pragma once
#include <string>
#include <map>

class Command
{
private:
	bool IsDigit(std::string str);


public:
	enum CommandTypeEnum { ADD, SUBTRACT, MULTIPLY, RESULT, QUIT, ERROR};
	std::map<std::string, CommandTypeEnum> CommandType = {
		{"add", ADD},
		{"subtract", SUBTRACT},
		{"multiply", MULTIPLY},
		{"result", RESULT},
		{"quit", QUIT},
		{"error", ERROR},

	};

	int Value;
	std::string Register;
	std::string Error;
	CommandTypeEnum Operation;

	Command(std::string, std::string, std::string);
	Command() {};
};

