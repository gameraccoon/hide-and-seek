#pragma once

#include <memory>

struct SDL_Window;

namespace HAL
{
	namespace Internal
	{
		class SdlWindow
		{
		public:
			SdlWindow(int width, int height);
			operator SDL_Window*();
			void show();

		private:
			std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> mSDLWindow;
		};
	}
}
