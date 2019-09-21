#include "Utils/Application/ArgumentsParser.h"

#include "Debug/Assert.h"

#include "AutoTests/Tests/CollidingCircularUnits/TestCase.h"

using CasesMap = std::map<std::string, std::function<void(const ArgumentsParser& arguments)>>;

CasesMap getCases()
{
	return CasesMap
	({
		{
			"CollidingCircularUnits", [](const ArgumentsParser& args)
			{
				 CollidingCircularUnitsTestCase testCase(800, 600);
				 testCase.start(args);
			}
		}
	});
}

int main(int argc, char** argv)
{
	ArgumentsParser arguments(argc, argv);

	unsigned int seed = 100500;
	if (arguments.hasArgument("randseed"))
	{
		std::string seedStr = arguments.getArgumentValue("randseed");
		seed = static_cast<unsigned int>(std::atoi(seedStr.c_str()));
	}

	std::srand(seed);

	auto cases = getCases();

	if (arguments.hasArgument("list-cases"))
	{
		for (const auto& casePair : cases)
		{
			std::cout << casePair.first << "\n";
		}
		return 0;
	}

	if (!arguments.hasArgument("case"))
	{
		Log::Instance().writeError("Test case name not provided");
		return 1;
	}

	auto caseIt = cases.find(arguments.getArgumentValue("case"));
	if (caseIt != cases.end())
	{
		caseIt->second(arguments);
	}
	else
	{
		Log::Instance().writeError("Unknown test " + arguments.getArgumentValue("case"));
		return 1;
	}

	return 0;
}
