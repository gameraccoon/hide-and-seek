#pragma once

#include <stdint.h>

/**
 * Interface for resource manager based on use counting
 */
class IUseCounter
{
public:
	using Uid = uint32_t;

public:
	virtual void beginUse(Uid uid) = 0;
	virtual void endUse(Uid uid) = 0;
};
