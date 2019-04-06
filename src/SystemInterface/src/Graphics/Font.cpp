#include "Graphics/Font.h"

#include "../Internal/SdlSurface.h"
#include "Base/Engine.h"
namespace Graphics
{
	Font::Base::~Base()
	{
		if (surface)
		{
			delete surface;
			surface = nullptr;
		}
	}

	Font::Font(SystemInterface::IUseCounter* useCounter, const Base* base)
        : Resource(useCounter, base ? base->uid : 0)
		, mBase(base)
	{
	}

	void Font::Draw(const char * text, float x, float y, float rotation)
	{
		mBase->engine->write(mBase->surface, text, x, y, rotation);
	}

	bool Font::isValid() const
	{
		return mBase != nullptr;
	}
}
