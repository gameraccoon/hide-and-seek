#pragma once

#include <string>
#include <vector>

class ArgumentsParser
{
public:
	ArgumentsParser(int argc, char **argv, const std::string& argumentSwitch = "--");

	bool hasArgument(const std::string& argument) const;
	const std::string& getArgumentValue(const std::string& argument) const;

private:
	std::vector <std::string> mTokens;
	const std::string mArgumentSwitch;
};
