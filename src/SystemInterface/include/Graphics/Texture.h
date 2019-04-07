#pragma once

#include <memory>

#include "Base/Resource.h"
#include <EngineFwd.h>

namespace SystemInterface
{
	namespace Internal
	{
		class SdlSurface;
	}
}

namespace Graphics
{
	class Texture : public SystemInterface::Resource
	{
	public:
		class Base : public Resource::Base
		{
		public:
			Base(SystemInterface::IUseCounter::Uid uid)
				: Resource::Base(uid)
			{
			}

			virtual ~Base();

			SystemInterface::Internal::SdlSurface* surface = nullptr;
			SystemInterface::Engine* engine;
		};

	public:
		Texture(SystemInterface::IUseCounter* useCounter, const Base* base);

		int getHeight() const;
		int getWidth() const;

		void draw(float x, float y, float ancX = 0.0f, float ancY = 0.0f, float scaleX = 0.0f, float scaleY  = 0.0f, float rotation = 0.0f, float alpha = 1.0f) const;

		virtual bool isValid() const override;

	private:
		const Texture::Base* mBase;
	};
}
