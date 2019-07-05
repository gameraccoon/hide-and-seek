#include "Utils/Application/ArgumentsParser.h"

#include "Debug/Log.h"

#include "AutoTests/Tests/CollidingCircularUnits/TestCase.h"

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

	if (!arguments.hasArgument("case"))
	{
		Log::Instance().writeError("test case name not provided");
		return 0;
	}

	if (arguments.getArgumentValue("case") == "CollidingCircularUnits")
	{
		CollidingCircularUnitsTestCase testCase;
		testCase.start(arguments);
	}

	return 0;
}
