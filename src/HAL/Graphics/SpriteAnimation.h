#pragma once

#include <memory>

#include "HAL/EngineFwd.h"

#include <vector>

#include "GameData/Core/ResourceHandle.h"

#include "HAL/Base/Resource.h"
#include "HAL/Base/Types.h"

#include "HAL/Graphics/Sprite.h"

namespace Graphics
{
	class SpriteAnimation : public HAL::Resource
	{
	public:
		SpriteAnimation() = default;
		explicit SpriteAnimation(std::vector<ResourceHandle>&& sprites);

		bool isValid() const override;

		const ResourceHandle& getSprite(float progress) const;
		const std::vector<ResourceHandle>& getSprites() const;

	private:
		std::vector<ResourceHandle> mSprites;
	};
}
