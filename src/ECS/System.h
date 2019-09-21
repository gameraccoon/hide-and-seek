#pragma once

#include <string>

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

	// for debug purposes
	virtual std::string getName() = 0;
};
