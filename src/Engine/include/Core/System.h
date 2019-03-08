#pragma once

#include <memory>
#include <Core/World.h>

/**
 * The base class for game Systems (ECS ideology)
 *
 * Abstract
 */
class System
{
public:
	virtual ~System() = default;

	virtual void update(World* world, float dt) = 0;
};
