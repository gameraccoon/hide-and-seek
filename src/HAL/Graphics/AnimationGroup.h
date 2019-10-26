#pragma once

#include <memory>
#include <vector>
#include <map>

#include "HAL/EngineFwd.h"

#include "GameData/Core/ResourceHandle.h"

#include "HAL/Base/Resource.h"
#include "HAL/Base/Types.h"

#include "HAL/Graphics/Sprite.h"

namespace Graphics
{
	class AnimationGroup : public HAL::Resource
	{
	public:
		AnimationGroup() = default;
		explicit AnimationGroup(std::map<std::string, std::vector<ResourceHandle>>&& animationClips, const std::string& stateMachineID, const std::string& defaultState);

		bool isValid() const override;
		const std::string& getStateMachineID() const { return mStateMachineID; }
		std::map<std::string, std::vector<ResourceHandle>> getAnimationClips() const { return mAnimationClips; }
		const std::string& getDefaultState() const { return mDefaultState; }

	private:
		std::map<std::string, std::vector<ResourceHandle>> mAnimationClips;
		std::string mStateMachineID;
		std::string mDefaultState;
	};
}
