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
	class Texture : public SystemInterface::Resource
	{
	public:
		Texture(SystemInterface::Internal::SdlSurface* surface);

		int getHeight() const;
		int getWidth() const;

		SystemInterface::Internal::SdlSurface* getSurface() const;

		virtual bool isValid() const override;

	private:
		SystemInterface::Internal::SdlSurface* mSurface = nullptr;
	};
}
