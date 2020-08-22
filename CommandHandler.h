#pragma once
#include <string>

#include "Command.h"

class CommandHandler
{


public:
	static Command GetCommand(std::string);
};

