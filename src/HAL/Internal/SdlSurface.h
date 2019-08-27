#pragma once

#include <string>

struct SDL_Surface;

namespace HAL
{
	namespace Internal
	{
		class Surface
		{
		public:
			explicit Surface(const std::string& filename);
			~Surface();

			int getWidth() const;
			int getHeight() const;

			SDL_Surface* getRawSurface();

		private:
			SDL_Surface* mSurface;
		};
	}
}
