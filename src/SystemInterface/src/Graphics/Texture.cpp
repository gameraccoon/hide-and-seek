#include "Graphics/Texture.h"

#include "../Internal/SdlSurface.h"
#include "Base/Engine.h"

namespace Graphics
{
	Texture::Base::~Base()
	{
		if (surface)
		{
			delete surface;
			surface = nullptr;
		}
	}

	Texture::Texture(SystemInterface::IUseCounter * useCounter, const Base* base)
		: Resource(useCounter, base->uid)
		, mBase(base)
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

	void Texture::draw(float x, float y, float ancX, float ancY, float scaleX, float scaleY,float rotation, float alpha) const
	{
		mBase->engine->render(mBase->surface, x, y, ancX, ancY, scaleX, scaleY, rotation, alpha);
	}

	bool Texture::isValid() const
	{
		return mBase->surface != nullptr;
	}
}
