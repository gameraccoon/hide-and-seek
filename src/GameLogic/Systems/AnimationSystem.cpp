#include "GameLogic/Systems/AnimationSystem.h"

#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/AnimationComponent.generated.h"
#include "GameData/World.h"

AnimationSystem::AnimationSystem(WorldHolder &worldHolder, const TimeData& time)
	: mWorldHolder(worldHolder)
	, mTime(time)
{
}

void AnimationSystem::update()
{
//	World* world = mWorldHolder.world;
//	float dt = mTime.dt;

//	world->getEntityManager().forEachComponentSet<AnimationComponent, SpriteComponent>([](AnimationComponent* animation, SpriteComponent* sprite){
//		std::vector<AnimationState<int>>& animationStates = animation->getDataRef();
//		for (animationStates)
//	});
}
