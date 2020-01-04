#pragma once

#include "Base/Types/String/Path.h"

#include "HAL/Base/Resource.h"

struct FC_Font;
struct SDL_Renderer;

namespace Graphics
{
	class Font : public HAL::Resource
	{
	public:
		Font() = default;

		explicit Font(const ResourcePath& path, int fontSize, SDL_Renderer* renderer);
		~Font() override;

		bool isValid() const override;

		FC_Font* getRawFont() const;

	private:
		FC_Font* mFont = nullptr;
	};
}
