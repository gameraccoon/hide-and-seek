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
		mSystemsManager->registerSystem<RenderSystem>(getEngine(), getResourceManager());

		Entity hero = mWorld->addEntity();
		auto heroTransformComponent = mWorld->addComponent<TransformComponent>(hero);
		heroTransformComponent->setLocation(Vector2D(10.0f, 10.0f));
		auto heroRenderComponent = mWorld->addComponent<RenderComponent>(hero);
		heroRenderComponent->setTexturePath("resources/textures/hero.png");
		mWorld->setPlayerControlledEntity(hero);

		Entity camera = mWorld->addEntity();
		mWorld->addComponent<CameraComponent>(camera);
		auto cameraTransformComponent = mWorld->addComponent<TransformComponent>(camera);
		cameraTransformComponent->setLocation(Vector2D(0.0f, 0.0f));
		mWorld->setMainCamera(camera);

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
