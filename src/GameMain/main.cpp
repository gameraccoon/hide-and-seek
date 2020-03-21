#include "Base/precomp.h"

#include "GameLogic/Game.h"

#include <time.h>

#include "Utils/Application/ArgumentsParser.h"

int main(int argc, char** argv)
{
	std::srand(static_cast<unsigned int>(time(nullptr)));

	ArgumentsParser arguments(argc, argv);

	Game game(800, 600);
	game.start(arguments);

	return 0;
}
