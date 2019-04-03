#define GLM_FORCE_RADIANS

#include <memory>

#include "StealthGame.h"

#include "Base/Engine.h"

#include <Modules/ActorFactory.h>

#include <Systems/RenderSystem.h>
#include <Systems/ControlSystem.h>
#include <Systems/CollisionSystem.h>

#include <Components/TransformComponent.h>
#include <Components/RenderComponent.h>
#include <Components/CollisionComponent.h>
#include <Components/MovementComponent.h>

namespace Game
{
	void StealthGame::start()
	{
		mWorld = std::make_unique<World>();

		mSystemsManager = std::make_unique<SystemsManager>();
		mSystemsManager->registerSystem<ControlSystem>(getEngine(), &mKeyStates);
		mSystemsManager->registerSystem<CollisionSystem>();
		mSystemsManager->registerSystem<RenderSystem>(getEngine(), getResourceManager());

		Entity hero = mWorld->addEntity();
		mWorld->setPlayerControlledEntity(hero);
		auto heroTransformComponent = mWorld->addComponent<TransformComponent>(hero);
		heroTransformComponent->setLocation(Vector2D(10.0f, 10.0f));
		auto heroCollisionComponent = mWorld->addComponent<CollisionComponent>(hero);
		Hull heroHull;
		heroHull.type = Hull::Type::Circular;
		heroHull.setRadius(32.0f);
		heroCollisionComponent->setGeometry(heroHull);
		mWorld->addComponent<MovementComponent>(hero);
		auto heroRenderComponent = mWorld->addComponent<RenderComponent>(hero, getResourceManager()->getTexture("resources/textures/hero.png"));
		heroRenderComponent->calcScaleFromSize(Vector2D(heroHull.getRadius()));

		Entity camera = mWorld->addEntity();
		mWorld->setMainCamera(camera);
		mWorld->addComponent<CameraComponent>(camera);
		auto cameraTransformComponent = mWorld->addComponent<TransformComponent>(camera);
		cameraTransformComponent->setLocation(Vector2D(0.0f, 0.0f));

		Entity wall = mWorld->addEntity();
		auto wallTransformComponent = mWorld->addComponent<TransformComponent>(wall);
		wallTransformComponent->setLocation(Vector2D(-60.0f, -60.0f));
		auto wallRenderComponent = mWorld->addComponent<RenderComponent>(wall, getResourceManager()->getTexture("resources/textures/testTexture.png"));
		wallRenderComponent->calcScaleFromSize(Vector2D(120.0f, 120.0f));
		auto wallCollisionComponent = mWorld->addComponent<CollisionComponent>(wall);
		Hull wallHull;
		wallHull.type = Hull::Type::Angular;
		wallHull.points.push_back(Vector2D(-60.0, -60.0));
		wallHull.points.push_back(Vector2D(-60.0, 60.0));
		wallHull.points.push_back(Vector2D(60.0, 60.0));
		wallHull.points.push_back(Vector2D(60.0, -60.0));
		wallHull.generateBorders();
		wallCollisionComponent->setGeometry(wallHull);

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
