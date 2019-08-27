#pragma once

namespace HAL
{
	namespace Internal
	{
		class SDLInstance
		{
		public:
			explicit SDLInstance(unsigned int flags);
			SDLInstance(const SDLInstance&) = delete;
			~SDLInstance();
		};
	}
}
