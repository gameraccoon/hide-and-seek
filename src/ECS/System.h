#pragma once

#include <memory>

class World;

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
