#pragma once

#include <memory>

#include "HAL/EngineFwd.h"

#include "HAL/Base/Resource.h"
#include "HAL/Base/Types.h"

namespace Graphics
{
	class Texture;

	class Sprite : public HAL::Resource
	{
	public:
		Sprite() = default;
		Sprite(const Graphics::Texture* texture, QuadUV uv);

		const Graphics::Texture* getTexture() const;
		QuadUV getUV() const { return mUV; }

		bool isValid() const override;

	private:
		const Graphics::Texture* mTexture = nullptr;
		QuadUV mUV;
	};
}
