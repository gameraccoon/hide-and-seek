#include "Base/precomp.h"

#include "HAL/Graphics/AnimationGroup.h"

#include "../Internal/SdlSurface.h"
#include "HAL/Base/Engine.h"

namespace Graphics
{
	AnimationGroup::AnimationGroup(std::map<StringID, std::vector<ResourceHandle>>&& animationClips, StringID stateMachineID, StringID defaultState)
		: mAnimationClips(animationClips)
		, mStateMachineID(stateMachineID)
		, mDefaultState(defaultState)
	{
	}

	bool AnimationGroup::isValid() const
	{
		return mStateMachineID.isValid();
	}
}
