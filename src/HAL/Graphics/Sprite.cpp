#include "HAL/Graphics/Sprite.h"

#include "HAL/Base/Engine.h"
#include "HAL/Graphics/Texture.h"

namespace Graphics
{
	Sprite::Sprite(const Graphics::Texture* texture, QuadUV uv)
		: mTexture(texture)
		, mUV(uv)
	{
	}

	const Graphics::Texture* Sprite::getTexture() const
	{
		return mTexture;
	}

	bool Sprite::isValid() const
	{
		return mTexture != nullptr;
	}
}
