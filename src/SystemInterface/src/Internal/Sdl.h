#pragma once

namespace SystemInterface
{
	namespace Internal
	{
		class Sdl
		{
		public:
			Sdl(unsigned int flags);
			~Sdl();

		private:
			Sdl(const Sdl&) = delete;
		};
	}
}
