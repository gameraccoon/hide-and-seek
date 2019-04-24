#include "Graphics/Font.h"

#include "../Internal/SdlSurface.h"
#include "Base/Engine.h"

namespace Graphics
{
	Font::Font(SystemInterface::Internal::SdlSurface* surface)
		: mSurface(surface)
	{
	}

	SystemInterface::Internal::SdlSurface*Font::getSurface() const
	{
		return mSurface;
	}

	bool Font::isValid() const
	{
		return mSurface != nullptr;
	}
}
