#pragma once

#include <string>

#include "HAL/Base/Resource.h"

struct SDL_Texture;
struct SDL_Renderer;

namespace Graphics
{
	class Texture : public HAL::Resource
	{
	public:
		Texture() = default;
		explicit Texture(const std::string& path, SDL_Renderer* renderer);
		~Texture() override;

		int getWidth() const;
		int getHeight() const;

		SDL_Texture* getRawTexture() const;

		virtual bool isValid() const override;

	private:
		SDL_Texture* mTexture = nullptr;
		int mWidth = 0;
		int mHeight = 0;
	};
}
