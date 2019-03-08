#include "Systems/ControlSystem.h"

#include <sdl/SDL_keycode.h>

#include <Components/RenderComponent.h>

ControlSystem::ControlSystem(SystemInterface::Engine* engine, KeyStatesMap* keyStatesMap)
	: mEngine(engine)
	, mKeyStatesMap(keyStatesMap)
{
}

void ControlSystem::update(World* world, float dt)
{
	const float speed = 50.0f;

	Vector2D direction(0.0f, 0.0f);
	if (auto it = mKeyStatesMap->find(SDLK_LEFT); it != mKeyStatesMap->end() && it->second)
	{
		direction += LEFT_DIRECTION;
	}

	if (auto it = mKeyStatesMap->find(SDLK_RIGHT); it != mKeyStatesMap->end() && it->second)
	{
		direction += RIGHT_DIRECTION;
	}

	if (auto it = mKeyStatesMap->find(SDLK_UP); it != mKeyStatesMap->end() && it->second)
	{
		direction += UP_DIRECTION;
	}

	if (auto it = mKeyStatesMap->find(SDLK_DOWN); it != mKeyStatesMap->end() && it->second)
	{
		direction += DOWN_DIRECTION;
	}

	if (direction != ZERO_VECTOR)
	{
		if (auto transform = world->getPlayerControlledTransform())
		{
			transform->setLocation(transform->getLocation() + direction * speed * dt);
		}
	}
}
