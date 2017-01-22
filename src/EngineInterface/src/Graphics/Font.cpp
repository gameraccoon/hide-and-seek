#include "Graphics/Font.h"

#include "../Internal/SdlSurface.h"
#include "../Internal/Engine.h"

Font::Base::~Base()
{
}

Font::Font(IUseCounter * useCounter, const Base* base)
	: Resource(useCounter, base ? base->uid : -1)
	, mBase(base)
{
}

Font::~Font()
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

