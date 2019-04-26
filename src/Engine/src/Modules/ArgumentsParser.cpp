#include "Modules/ArgumentsParser.h"

#include <algorithm>

ArgumentsParser::ArgumentsParser(int argc, char** argv, const std::string& argumentSwitch)
	: mArgumentSwitch(argumentSwitch)
{
	for (int i = 1; i < argc; ++i)
	{
		mTokens.push_back(std::string(argv[i]));
	}
}

const std::string&ArgumentsParser::getArgumentValue(const std::string& argument) const
{
	static const std::string empty_string("");

	std::vector<std::string>::const_iterator itr;

	itr = std::find(this->mTokens.begin(), this->mTokens.end(), mArgumentSwitch + argument);

	if (itr != this->mTokens.end())
	{
		++itr;
		if (itr != this->mTokens.end())
		{
			return *itr;
		}
	}

	return empty_string;
}

bool ArgumentsParser::hasArgument(const std::string& argument) const
{
	return std::find(this->mTokens.begin(), this->mTokens.end(), mArgumentSwitch + argument) != this->mTokens.end();
}
