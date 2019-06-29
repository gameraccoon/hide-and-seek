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
	class Font : public HAL::Resource
	{
	public:
		Font(HAL::Internal::SdlSurface* surface);

		HAL::Internal::SdlSurface* getSurface() const;

		virtual bool isValid() const override;

	private:
		HAL::Internal::SdlSurface* mSurface = nullptr;
	};
}
