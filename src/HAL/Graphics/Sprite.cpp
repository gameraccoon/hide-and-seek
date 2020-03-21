#include "Base/precomp.h"

#include "HAL/Graphics/Sprite.h"

#include "HAL/Internal/SdlSurface.h"
#include "HAL/Base/Engine.h"

namespace Graphics
{
	Sprite::Sprite(const Internal::Surface* surface, QuadUV uv)
		: mSurface(surface)
		, mUV(uv)
	{
	}

	int Sprite::getHeight() const
	{
		return mSurface->getHeight();
	}

	int Sprite::getWidth() const
	{
		return mSurface->getWidth();
	}

	const Internal::Surface* Sprite::getSurface() const
	{
		return mSurface;
	}

	bool Sprite::isValid() const
	{
		return mSurface != nullptr;
	}
}
