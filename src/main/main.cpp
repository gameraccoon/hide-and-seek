#include <StealthGame.h>

#include <Utils/ArgumentsParser.h>

int main(int argc, char** argv)
{
	std::srand(static_cast<unsigned int>(time(nullptr)));

	ArgumentsParser arguments(argc, argv);

	Game::StealthGame game;
	game.start(arguments);

	return 0;
}
