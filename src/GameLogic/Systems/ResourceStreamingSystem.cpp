#include "GameLogic/Systems/ResourceStreamingSystem.h"

#include "GameData/Components/SpriteComponent.generated.h"
#include "GameData/Components/AnimationDataComponent.generated.h"

#include "GameData/World.h"
#include "GameData/GameData.h"


ResourceStreamingSystem::ResourceStreamingSystem(WorldHolder& worldHolder, HAL::ResourceManager* resourceManager)
	: mWorldHolder(worldHolder)
	, mResourceManager(resourceManager)
{
}

void ResourceStreamingSystem::update()
{
	World* world = mWorldHolder.world;
	GameData* gameData = mWorldHolder.gameData;

	world->getEntityManger().forEachComponentSet<SpriteComponent>([&resourceManager = mResourceManager](SpriteComponent* sprite)
	{
		if (ResourceHandle textureHandle = sprite->getSpriteHandle(); !textureHandle.isValid())
		{
			sprite->setSpriteHandle(resourceManager->lockSprite(sprite->getTexturePath()));
		}
	});

	// ToDo: this can be optimized
	auto [animationData] = gameData->getGameComponents().getComponents<AnimationDataComponent>();
	if (animationData)
	{
		for (auto& data : animationData->getDatasRef())
		{
			for(auto& staticData : data.second.animDatas)
			{
				if (!staticData.second.animation.isValid())
				{
					staticData.second.animation = mResourceManager->lockSpriteAnimation(staticData.second.globalAnimId);
				}
			}
		}
	}
}
