#pragma once

#include <memory>

struct SDL_Window;

namespace Engine
{
	namespace Internal
	{
		class SdlWindow {
		public:
			SdlWindow(unsigned int width, unsigned int height);
			operator SDL_Window*();
			void Show();

		private:
			std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> mSDLWindow;
		};
	}
}
