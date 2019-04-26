#include <StealthGame.h>
#include <Modules/ArgumentsParser.h>

int main(int argc, char** argv)
{
	ArgumentsParser arguments(argc, argv);

	Game::StealthGame game;
	game.start(arguments);

	return 0;
}
