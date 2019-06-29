#pragma once

#include <memory>

#include "HAL/EngineFwd.h"

#include "HAL/Base/Resource.h"

namespace HAL
{
	namespace Internal
	{
		class SdlSurface;
	}
}

namespace Graphics
{
	class Texture : public HAL::Resource
	{
	public:
		Texture(HAL::Internal::SdlSurface* surface);

		int getHeight() const;
		int getWidth() const;

		HAL::Internal::SdlSurface* getSurface() const;

		virtual bool isValid() const override;

	private:
		HAL::Internal::SdlSurface* mSurface = nullptr;
	};
}