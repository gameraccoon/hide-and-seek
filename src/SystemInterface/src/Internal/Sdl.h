#pragma once

namespace SystemInterface
{
	namespace Internal
	{
		class Sdl
		{
		public:
			Sdl(int flags);
			~Sdl();

		private:
			Sdl(const Sdl&) = delete;
		};
	}
}
