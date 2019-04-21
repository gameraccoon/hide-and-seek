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
			Base(SystemInterface::IUseCounter::Uid uid)
				: Resource::Base(uid)
			{
			}

			virtual ~Base();

			// because std::unique_ptr doesn't like fwd
			SystemInterface::Internal::SdlSurface* surface = nullptr;
			SystemInterface::Engine* engine = nullptr;
		};

	public:
		Font(SystemInterface::IUseCounter* useCounter, const Base* base);

		void Draw(const char* text, float x, float y, float rotation = 0.0f);

		virtual bool isValid() const override;

	private:
		const Font::Base* mBase;
	};
}
