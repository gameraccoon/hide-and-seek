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

	TransformComponent* cameraTransformComponent = std::get<0>(world->getEntityComponents<TransformComponent>(mainCamera.getEntity()));
	if (cameraTransformComponent == nullptr)
	{
		return;
	}

	Vector2D screenHalfSize = Vector2D(mEngine->getWidth(), mEngine->getHeight()) * 0.5f;
	Vector2D mouseScreenPos(mEngine->getMouseX(), mEngine->getMouseY());

	Vector2D drawShift = screenHalfSize - cameraTransformComponent->getLocation();

	Vector2D controlledEntityPosition(0.0f, 0.0f);
	if (NullableEntity controlledEntity = world->getPlayerControlledEntity(); controlledEntity.isValid())
	{
		TransformComponent* transform;
		MovementComponent* movement;
		std::tie(transform, movement) = world->getEntityComponents<TransformComponent, MovementComponent>(controlledEntity.getEntity());

		if (transform != nullptr && movement != nullptr)
		{
			Vector2D oldLocation = transform->getLocation();
			Vector2D move = speed * movementDirection;
			movement->setSpeed(move);
			transform->setLocation(oldLocation + move * dt);

			Vector2D direction = mouseScreenPos - oldLocation - drawShift;
			transform->setRotation(direction.rotation());
		}
	}

	if (NullableEntity mainCamera = world->getMainCamera(); mainCamera.isValid())
	{
		if (TransformComponent* cameraTransform = std::get<0>(world->getEntityComponents<TransformComponent>(mainCamera.getEntity())))
		{
			Vector2D cameraLocation = cameraTransform->getLocation();
			Vector2D mouseWorldPos = mouseScreenPos - cameraLocation - screenHalfSize;
			cameraTransform->setLocation((mouseWorldPos + controlledEntityPosition) * 0.5f);
		}
	}
}
