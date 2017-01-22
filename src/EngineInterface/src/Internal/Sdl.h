#pragma once

namespace Engine
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
