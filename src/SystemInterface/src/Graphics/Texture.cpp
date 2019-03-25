#include "Graphics/Texture.h"

#include "../Internal/SdlSurface.h"
#include "Base/Engine.h"

namespace Graphics
{
	Texture::Base::~Base()
	{
	}

	Texture::Texture(SystemInterface::IUseCounter * useCounter, const Base* base)
        : Resource(useCounter, base ? base->uid : 0)
		, mBase(base)
	{
	}

	Texture::~Texture()
	{
	}

	int Texture::getHeight() const
	{
		return mBase->surface->height();
	}

	int Texture::getWidth() const
	{
		return mBase->surface->width();
	}

	void Texture::draw(float x, float y, float ancX, float ancY, float rotation, float alpha)
	{
		mBase->engine->render(mBase->surface, x, y, ancX, ancY, rotation, alpha);
	}

	bool Texture::isValid() const
	{
		return mBase != nullptr;
	}
}
