#pragma once

namespace SystemInterface
{
	/**
	 * Base class for any resource type
	 */
	class Resource
	{
	public:
		virtual ~Resource() = default;
		virtual bool isValid() const = 0;
	};
}
