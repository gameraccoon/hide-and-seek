#include "GameLogic/Systems/AnimationSystem.h"

#include <algorithm>

#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/AnimationDataComponent.generated.h"
#include "GameData/World.h"

AnimationSystem::AnimationSystem(WorldHolder &worldHolder, const TimeData& time)
	: mWorldHolder(worldHolder)
	, mTime(time)
{
}

void AnimationSystem::update()
{
	World* world = mWorldHolder.world;
	float dt = mTime.dt;

	world->getEntityManager().forEachComponentSet<AnimationDataComponent, RenderComponent>([dt](AnimationDataComponent* animationData, RenderComponent* render)
	{
		std::vector<AnimationData>& animationDatas = animationData->getDatasRef();
		for (auto& data : animationDatas)
		{
			data.progress += data.params.speed * dt;
			if (data.progress >= 1.0f && data.params.isLooped)
			{
				data.progress -= 1.0f;
			}

			size_t spriteIdx = 0;

			const auto& ids = render->getSpriteIds();
			for (size_t i = 0; i < render->getSpriteIds().size(); ++i)
			{
				if (ids[i] == data.spriteId)
				{
					spriteIdx = i;
				}
			}

			render->getSpriteDatasRef()[spriteIdx].spriteHandle = data.sprites[std::min(static_cast<size_t>(data.sprites.size() * data.progress), data.sprites.size())];
		}
	});
}
