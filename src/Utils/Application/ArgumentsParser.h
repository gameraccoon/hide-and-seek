#pragma once

#include <string>
#include <vector>

class ArgumentsParser
{
public:
	ArgumentsParser(int argc, char **argv, const std::string& argumentSwitch = "--");

	bool hasArgument(const std::string& argument) const;
	std::string getArgumentValue(const std::string& argument, const std::string& defaultValue = "") const;

private:
	std::vector <std::string> mTokens;
	const std::string mArgumentSwitch;
};
