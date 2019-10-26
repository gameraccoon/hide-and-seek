#include "HAL/Graphics/AnimationGroup.h"

#include "../Internal/SdlSurface.h"
#include "HAL/Base/Engine.h"

namespace Graphics
{
	AnimationGroup::AnimationGroup(std::map<std::string, std::vector<ResourceHandle>>&& animationClips, const std::string& stateMachineID, const std::string& defaultState)
		: mAnimationClips(animationClips)
		, mStateMachineID(stateMachineID)
		, mDefaultState(defaultState)
	{
	}

	bool AnimationGroup::isValid() const
	{
		return !mStateMachineID.empty();
	}
}
