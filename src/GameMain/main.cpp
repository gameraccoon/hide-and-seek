#include "Base/precomp.h"

#include "Base/Random/Random.h"

#include "GameLogic/Game.h"

#include <time.h>

#include "Utils/Application/ArgumentsParser.h"

int main(int argc, char** argv)
{
	Random::GlobalGenerator = std::mt19937(time(nullptr));

	ArgumentsParser arguments(argc, argv);

	Game game(800, 600);
	game.start(arguments);

	return 0;
}
