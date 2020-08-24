#include <string>
#include <regex>

#include "CommandHandler.h"
#include "Command.h"

Command CommandHandler::GetCommand(std::string newInput)
{
	using namespace std::regex_constants;

	Command testCommand("", "error", "");
	std::smatch matches;

	std::regex registerRegex("(\\w*) (\\w*) (\\w*)");
	std::regex printRegex("print (\\w*)", ECMAScript | icase);

	// Borrowed from https://stackoverflow.com/questions/313970/how-to-convert-stdstring-to-lower-case
	std::transform(newInput.begin(), newInput.end(), newInput.begin(),
		[](unsigned char c) { return std::tolower(c); });

	if (std::regex_match(newInput.c_str(), printRegex))
	{
		std::regex_search(newInput, matches, printRegex);
		Command printCommand(matches[1].str(), "print", "");
		return printCommand;
	}
	else if (std::regex_match(newInput.c_str(), registerRegex))
	{
		std::regex_search(newInput, matches, registerRegex);
		Command registerCommand(matches[1].str(), matches[2].str(), matches[3].str());
		return registerCommand;
	}
	else if (newInput.compare("quit") == 0 || newInput.compare("exit") == 0)
	{
		Command quitCommand("", "quit", "");
		return quitCommand;
	}

	return testCommand;
}