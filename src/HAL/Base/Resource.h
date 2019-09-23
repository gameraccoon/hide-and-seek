#pragma once

namespace HAL
{
	/**
	 * Base class for any resource type
	 */
	class Resource
	{
	public:
		Resource() = default;
		Resource(const Resource&) = delete;
		Resource& operator=(Resource&) = delete;

		virtual ~Resource() = default;
		virtual bool isValid() const = 0;
	};
}
