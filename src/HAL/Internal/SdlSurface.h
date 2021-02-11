#pragma once

#include <string>
#include <memory>

#include "HAL/Base/Resource.h"

struct SDL_Surface;

namespace Graphics
{
	namespace Internal
	{
		class Surface : public HAL::Resource
		{
		public:
			explicit Surface(const std::string& filename);

			Surface(const Surface&) = delete;
			Surface& operator=(const Surface&) = delete;
			Surface(Surface&&) = delete;
			Surface& operator=(Surface&&) = delete;

			~Surface();

			int getWidth() const;
			int getHeight() const;

			void bind() const;

			virtual bool isValid() const override;

		private:
			SDL_Surface* mSurface;
			unsigned int mTextureID;
		};
	}
}
