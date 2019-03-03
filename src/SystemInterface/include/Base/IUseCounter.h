#pragma once

#include <stdint.h>

namespace SystemInterface
{
	/**
	 * Interface for resource manager based on use counting
	 */
	class IUseCounter
	{
	public:
		virtual ~IUseCounter() = default;

		using Uid = uint32_t;

	public:
		virtual void beginUse(Uid uid) = 0;
		virtual void endUse(Uid uid) = 0;
	};
}
