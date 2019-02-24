#include <StealthGame.h>
#include <actorsRegistration.h>

int main(int, char**)
{
	FactoryActors::RegisterAll();

    Game::StealthGame game;
    game.start();

	return 0;
}
