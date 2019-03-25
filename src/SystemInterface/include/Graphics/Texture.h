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
			Base(SystemInterface::IUseCounter::Uid uid, Destructor destructor)
				: Resource::Base(uid, destructor)
			{
			}

			virtual ~Base();

			// because std::unique_ptr doesn't like fwd
			SystemInterface::Internal::SdlSurface* surface;
			SystemInterface::Engine* engine;
		};

	public:
		Texture(SystemInterface::IUseCounter* useCounter, const Base* base);
		Texture(const Texture&) = default;
		Texture(Texture&&) = default;
        virtual ~Texture() override;

		int getHeight() const;
		int getWidth() const;

		void draw(float x, float y, float ancX = 0.0f, float ancY = 0.0f, float rotation = 0.0f, float alpha = 1.0f);

		virtual bool isValid() const override;

	private:
		const Texture::Base* mBase;
	};
}
