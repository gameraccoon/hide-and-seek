#include "Graphics/Texture.h"

#include "../Internal/SdlSurface.h"
#include "Base/Engine.h"

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
	return mBase->surface->height();
}

int Texture::GetWidth() const
{
	return mBase->surface->width();
}

void Texture::Draw(float x, float y, float rotation, float alpha)
{
	mBase->engine->render(mBase->surface, x, y, rotation, alpha);
}

bool Texture::isValid() const
{
	return mBase != nullptr;
}
