#include "GameLogic/Initialization/Animations.h"

#include "GameData/Enums/CharacterAnimationState.generated.h"
#include "GameData/Enums/CharacterLegsAnimationState.generated.h"

namespace Animations
{
	static void RegisterCharacterAnimations(AnimationComponent* /*animationData*/)
	{
/*		AnimationState<int> characterAnimationData;

		AnimationState<int>::AnimData idleAnimData;
		idleAnimData.globalAnimId = "animations/character/handgun_idle.json";
		idleAnimData.speed = 1.0f;
		idleAnimData.isLooped = true;
		characterAnimationData.animDatas.emplace(static_cast<int>(CharacterAnimationState::Idle), std::move(idleAnimData));

		AnimationData<int>::AnimData moveAnimData;
		moveAnimData.globalAnimId = "animations/character/handgun_move.json";
		moveAnimData.speed = 1.0f;
		moveAnimData.isLooped = true;
		characterAnimationData.animDatas.emplace(static_cast<int>(CharacterAnimationState::Move), std::move(moveAnimData));

		AnimationData<int>::AnimData shootAnimData;
		shootAnimData.globalAnimId = "animations/character/handgun_shoot.json";
		shootAnimData.speed = 1.0f;
		shootAnimData.isLooped = false;
		characterAnimationData.animDatas.emplace(static_cast<int>(CharacterAnimationState::Shoot), std::move(shootAnimData));

		AnimationData<int>::AnimData reloadAnimData;
		reloadAnimData.globalAnimId = "animations/character/handgun_reload.json";
		reloadAnimData.speed = 1.0f;
		reloadAnimData.isLooped = false;
		characterAnimationData.animDatas.emplace(static_cast<int>(CharacterAnimationState::Reload), std::move(reloadAnimData));

		animationData->getDatasRef().emplace(get_enum_name<CharacterAnimationState>(), std::move(characterAnimationData));
*/	}

	static void RegisterCharacterLegsAnimations(AnimationComponent* /*animationData*/)
	{

	}

	void RegisterAnimations(AnimationComponent* animationData)
	{
		if (!animationData)
		{
			return;
		}

		RegisterCharacterAnimations(animationData);
		RegisterCharacterLegsAnimations(animationData);
	}
}
