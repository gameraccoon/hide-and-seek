#pragma once

#include <memory>
#include <vector>
#include <map>

#include "Base/String/StringID.h"

#include "GameData/Core/ResourceHandle.h"

#include "HAL/EngineFwd.h"
#include "HAL/Base/Resource.h"
#include "HAL/Base/Types.h"
#include "HAL/Graphics/Sprite.h"

namespace Graphics
{
	class AnimationGroup : public HAL::Resource
	{
	public:
		AnimationGroup() = default;
		explicit AnimationGroup(std::map<StringID, std::vector<ResourceHandle>>&& animationClips, StringID stateMachineID, StringID defaultState);

		bool isValid() const override;
		StringID getStateMachineID() const { return mStateMachineID; }
		std::map<StringID, std::vector<ResourceHandle>> getAnimationClips() const { return mAnimationClips; }
		StringID getDefaultState() const { return mDefaultState; }

	private:
		std::map<StringID, std::vector<ResourceHandle>> mAnimationClips;
		StringID mStateMachineID;
		StringID mDefaultState;
	};
}
