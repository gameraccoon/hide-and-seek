#pragma once

#include <memory>

#include "HAL/EngineFwd.h"

#include "HAL/Base/Resource.h"
#include "HAL/Base/Types.h"

namespace HAL
{
	namespace Internal
	{
		class SdlSurface;
	}
}

namespace Graphics
{
	class Sprite : public HAL::Resource
	{
	public:
		Sprite(HAL::Internal::SdlSurface* surface, QuadUV uv);

		int getHeight() const;
		int getWidth() const;

		HAL::Internal::SdlSurface* getSurface() const;
		QuadUV getUV() const { return mUV; }

		virtual bool isValid() const override;

	private:
		HAL::Internal::SdlSurface* mSurface = nullptr;
		QuadUV mUV;
	};
}
