#include "HAL/Graphics/Font.h"

#include "../Internal/SdlSurface.h"
#include "HAL/Base/Engine.h"

namespace Graphics
{
	Font::Font(HAL::Internal::SdlSurface* surface)
		: mSurface(surface)
	{
	}

	HAL::Internal::SdlSurface* Font::getSurface() const
	{
		return mSurface;
	}

	bool Font::isValid() const
	{
		return mSurface != nullptr;
	}
}
