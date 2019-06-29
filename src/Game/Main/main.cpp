#include "Game/Game.h"

#include "Utils/Application/ArgumentsParser.h"

int main(int argc, char** argv)
{
	std::srand(static_cast<unsigned int>(time(nullptr)));

	ArgumentsParser arguments(argc, argv);

	Game game;
	game.start(arguments);

	return 0;
}
