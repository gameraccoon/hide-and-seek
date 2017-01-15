#include "Texture.h"

#include "../Internal/SdlSurface.h"
#include "../Internal/Engine.h"

Texture::Base::~Base()
{
}

Texture::Texture(IUseCounter * useCounter, const Base* base)
	: Resource(useCounter, base ? base->uid : -1)
	, mBase(base)
{
}

Texture::~Texture()
{
}

int Texture::GetHeight() const
{
	return mBase->surface->Height();
}

int Texture::GetWidth() const
{
	return mBase->surface->Width();
}

void Texture::Draw(int x, int y)
{
	mBase->engine->Render(mBase->surface, (float)x, (float)y);
}
