#define GLM_FORCE_RADIANS

#include <memory>

#include "StealthGame.h"

#include "Base/Engine.h"

#include <Modules/ActorFactory.h>

#include <Systems/RenderSystem.h>
#include <Systems/ControlSystem.h>

#include <Components/TransformComponent.h>
#include <Components/RenderComponent.h>

namespace Game
{
	void StealthGame::start()
	{
		mWorld = std::make_unique<World>();

		mSystemsManager = std::make_unique<SystemsManager>();
		mSystemsManager->registerSystem<ControlSystem>(getEngine(), &mKeyStates);
		mSystemsManager->registerSystem<RenderSystem>(getResourceManager());

		Entity hero = mWorld->addEntity();
		auto transformComponent = mWorld->addComponent<TransformComponent>(hero);
		transformComponent->setLocation(Vector2D(10, 10));
		auto renderComponent = mWorld->addComponent<RenderComponent>(hero);
		renderComponent->setTexturePath("resources/textures/hero.png");

		mWorld->setPlayerControlledEntity(hero);

		// start the main loop
		getEngine()->start(this);
	}

	void StealthGame::setKeyState(int key, bool isPressed)
	{
		mKeyStates[key] = isPressed;
	}

	void StealthGame::update(float dt)
	{
		mSystemsManager->update(mWorld.get(), dt);
	}
}
