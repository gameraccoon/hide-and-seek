#include "StealthGame.h"

#include <memory>

#include "Base/Engine.h"

#include <Modules/LevelLoader.h>

#include <Systems/RenderSystem.h>
#include <Systems/ControlSystem.h>
#include <Systems/CollisionSystem.h>

#include <Components/TransformComponent.h>
#include <Components/RenderComponent.h>
#include <Components/CollisionComponent.h>
#include <Components/MovementComponent.h>
#include <Components/CameraComponent.h>
#include <Components/LightComponent.h>

namespace Game
{
	void StealthGame::start()
	{
		mSystemsManager.registerSystem<ControlSystem>(getEngine(), &mKeyStates);
		mSystemsManager.registerSystem<CollisionSystem>();
		mSystemsManager.registerSystem<RenderSystem>(getEngine(), getResourceManager());

		mComponentFactory.registerComponent<TransformComponent>();
		mComponentFactory.registerComponent<RenderComponent>();
		mComponentFactory.registerComponent<CollisionComponent>();
		mComponentFactory.registerComponent<MovementComponent>();
		mComponentFactory.registerComponent<CameraComponent>();
		mComponentFactory.registerComponent<LightComponent>();

		Entity hero = mWorld.getEntityManger().addEntity();
		mWorld.setPlayerControlledEntity(hero);
		auto heroTransformComponent = mWorld.getEntityManger().addComponent<TransformComponent>(hero);
		heroTransformComponent->setLocation(Vector2D(10.0f, 10.0f));
		auto heroCollisionComponent = mWorld.getEntityManger().addComponent<CollisionComponent>(hero);
		Hull heroHull;
		heroHull.type = Hull::Type::Circular;
		heroHull.setRadius(32.0f);
		heroCollisionComponent->setGeometry(heroHull);
		mWorld.getEntityManger().addComponent<MovementComponent>(hero);
		auto heroRenderComponent = mWorld.getEntityManger().addComponent<RenderComponent>(hero);
		heroRenderComponent->setTexture(getResourceManager()->getTexture("resources/textures/hero.png"));
		heroRenderComponent->calcScaleFromSize(Vector2D(heroHull.getRadius(), heroHull.getRadius()));

		Entity camera = mWorld.getEntityManger().addEntity();
		mWorld.setMainCamera(camera);
		mWorld.getEntityManger().addComponent<CameraComponent>(camera);
		auto cameraTransformComponent = mWorld.getEntityManger().addComponent<TransformComponent>(camera);
		cameraTransformComponent->setLocation(Vector2D(0.0f, 0.0f));

		Entity wall = mWorld.getEntityManger().addEntity();
		auto wallTransformComponent = mWorld.getEntityManger().addComponent<TransformComponent>(wall);
		wallTransformComponent->setLocation(Vector2D(-60.0f, -60.0f));
		auto wallRenderComponent = mWorld.getEntityManger().addComponent<RenderComponent>(wall);
		wallRenderComponent->setTexture(getResourceManager()->getTexture("resources/textures/testTexture.png"));
		wallRenderComponent->calcScaleFromSize(Vector2D(120.0f, 120.0f));
		auto wallCollisionComponent = mWorld.getEntityManger().addComponent<CollisionComponent>(wall);
		Hull wallHull;
		wallHull.type = Hull::Type::Angular;
		wallHull.points.push_back(Vector2D(60.0, -60.0));
		wallHull.points.push_back(Vector2D(60.0, 60.0));
		wallHull.points.push_back(Vector2D(-60.0, 60.0));
		wallHull.points.push_back(Vector2D(-60.0, -60.0));
		wallHull.generateBorders();
		wallCollisionComponent->setGeometry(wallHull);

		LevelLoader::SaveWorld(mWorld, "test", mComponentFactory);
		mWorld = World();
		LevelLoader::LoadWorld(mWorld, "test", mComponentFactory);

		// start the main loop
		getEngine()->start(this);
	}

	void StealthGame::setKeyState(int key, bool isPressed)
	{
		mKeyStates[key] = isPressed;
	}

	void StealthGame::update(float dt)
	{
		mSystemsManager.update(&mWorld, dt);
	}
}
