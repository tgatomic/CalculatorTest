#include <string>
#include <regex>

#include "CommandHandler.h"
#include "Command.h"

Command CommandHandler::GetCommand(std::string newInput)
{

	//PARSE THAT COMMAND

	std::string input = "A add random";
	std::regex r("(\\w*) (\\w*) (\\w*)");

	std::smatch matches;
	std::regex_search(newInput, matches, r);

	std::string test =  matches[1].str();

	Command testCommand(matches[1].str(), matches[2].str(), matches[3].str());

	return testCommand;
}