#include "Systems/ControlSystem.h"

#include <sdl/SDL_keycode.h>

#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>
#include <Components/MovementComponent.h>

ControlSystem::ControlSystem(SystemInterface::Engine* engine, KeyStatesMap* keyStatesMap)
	: mEngine(engine)
	, mKeyStatesMap(keyStatesMap)
{
}

void ControlSystem::update(World* world, float dt)
{
	const float speed = 50.0f;

	Vector2D movementDirection(0.0f, 0.0f);
	if (auto it = mKeyStatesMap->find(SDLK_LEFT); it != mKeyStatesMap->end() && it->second)
	{
		movementDirection += LEFT_DIRECTION;
	}

	if (auto it = mKeyStatesMap->find(SDLK_RIGHT); it != mKeyStatesMap->end() && it->second)
	{
		movementDirection += RIGHT_DIRECTION;
	}

	if (auto it = mKeyStatesMap->find(SDLK_UP); it != mKeyStatesMap->end() && it->second)
	{
		movementDirection += UP_DIRECTION;
	}

	if (auto it = mKeyStatesMap->find(SDLK_DOWN); it != mKeyStatesMap->end() && it->second)
	{
		movementDirection += DOWN_DIRECTION;
	}

	NullableEntity mainCamera = world->getMainCamera();
	if (!mainCamera.isValid())
	{
		return;
	}

	auto [cameraTransform] = world->getEntityManger().getEntityComponents<TransformComponent>(mainCamera.getEntity());
	if (cameraTransform == nullptr)
	{
		return;
	}

	Vector2D screenHalfSize = Vector2D(static_cast<float>(mEngine->getWidth()), static_cast<float>(mEngine->getHeight())) * 0.5f;
	Vector2D mouseScreenPos(mEngine->getMouseX(), mEngine->getMouseY());

	Vector2D drawShift = screenHalfSize - cameraTransform->getLocation();

	if (NullableEntity controlledEntity = world->getPlayerControlledEntity(); controlledEntity.isValid())
	{
		auto [transform, movement] = world->getEntityManger().getEntityComponents<TransformComponent, MovementComponent>(controlledEntity.getEntity());

		if (transform != nullptr && movement != nullptr)
		{
			Vector2D oldLocation = transform->getLocation();
			Vector2D move = speed * movementDirection;
			movement->setSpeed(move);
			Vector2D controlledEntityPosition = oldLocation + move * dt;
			transform->setLocation(controlledEntityPosition);

			Vector2D direction = mouseScreenPos - oldLocation - drawShift;
			transform->setRotation(direction.rotation());

			// main camera follows the player
			cameraTransform->setLocation(controlledEntityPosition);
		}
	}
}
