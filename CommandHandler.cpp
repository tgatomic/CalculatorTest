#include <string>
#include <regex>

#include "CommandHandler.h"
#include "Command.h"

Command CommandHandler::GetCommand(std::string newInput)
{
	std::regex r("(\\w*) (\\w*) (\\w*)");
	std::smatch matches;
	std::regex_search(newInput, matches, r);

	Command testCommand(matches[1].str(), matches[2].str(), matches[3].str());

	return testCommand;
}