#include "HAL/Graphics/SpriteAnimation.h"

#include "../Internal/SdlSurface.h"
#include "HAL/Base/Engine.h"

namespace Graphics
{
	SpriteAnimation::SpriteAnimation(std::vector<ResourceHandle>&& sprites)
		: mSprites(sprites)
	{
	}

	bool SpriteAnimation::isValid() const
	{
		return !mSprites.empty();
	}

	const ResourceHandle& SpriteAnimation::getSprite(float progress) const
	{
		return mSprites[std::min(mSprites.size()*static_cast<size_t>(progress), mSprites.size() - 1)];
	}

	const std::vector<ResourceHandle> &SpriteAnimation::getSprites() const
	{
		return mSprites;
	}
}
