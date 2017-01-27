#pragma once

#include <memory>

struct SDL_Window;

namespace SystemInterface
{
	namespace Internal
	{
		class SdlWindow
		{
		public:
			SdlWindow(unsigned int width, unsigned int height);
			operator SDL_Window*();
			void show();

		private:
			std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> mSDLWindow;
		};
	}
}
