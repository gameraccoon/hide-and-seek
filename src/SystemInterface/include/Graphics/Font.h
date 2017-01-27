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
	class Font : public SystemInterface::Resource
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
		Font(SystemInterface::IUseCounter* useCounter, const Base* base);
		Font(const Font&) = default;
		Font(Font&&) = default;
		virtual ~Font();

		void Draw(const char* text, float x, float y, float rotation = 0.0f);

		virtual bool isValid() const override;

	private:
		const Font::Base* mBase;
	};
}
