#include "Systems/ControlSystem.h"

#include <sdl/SDL_keycode.h>

#include <Components/RenderComponent.h>
#include <Components/TransformComponent.h>

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

	if (NullableEntity controlledEntity = world->getPlayerControlledEntity(); controlledEntity.isValid())
	{
		if (TransformComponent::Ptr transform = std::get<0>(world->getEntityComponents<TransformComponent>(controlledEntity.getEntity())))
		{
			if (movementDirection != ZERO_VECTOR)
			{
				transform->setLocation(transform->getLocation() + movementDirection * speed * dt);
			}

			Vector2D mousePos(mEngine->getMouseX(), mEngine->getMouseY());
			Vector2D direction = mousePos - transform->getLocation();
			transform->setRotation(direction.rotation());
		}
	}
}
