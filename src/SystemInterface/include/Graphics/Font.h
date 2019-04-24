#pragma once

#include <memory>

#include <EngineFwd.h>

#include "Base/Resource.h"

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
		Font(SystemInterface::Internal::SdlSurface* surface);

		SystemInterface::Internal::SdlSurface* getSurface() const;

		virtual bool isValid() const override;

	private:
		SystemInterface::Internal::SdlSurface* mSurface = nullptr;
	};
}
