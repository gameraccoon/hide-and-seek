#pragma once

#include "Base/String/Path.h"

#include "HAL/Base/Resource.h"

struct SDL_Texture;
struct SDL_Renderer;

namespace Graphics
{
	class Texture : public HAL::Resource
	{
	public:
		Texture() = default;
		explicit Texture(const ResourcePath& path, SDL_Renderer* renderer);
		~Texture() override;

		bool isValid() const override;

		int getHeight() const;
		int getWidth() const;

		SDL_Texture* getRawTexture() const;

	private:
		SDL_Texture* mTexture = nullptr;
		int mWidth = 0;
		int mHeight = 0;
	};
}
