#pragma once

#include <memory>

/**
 * The base class for game Systems (ECS ideology)
 *
 * Abstract
 */
class System
{
public:
	virtual ~System() = default;

	virtual void update() = 0;
};
