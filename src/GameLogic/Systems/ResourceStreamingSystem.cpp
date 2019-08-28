#include "GameLogic/Systems/ResourceStreamingSystem.h"

#include "GameData/Components/SpriteComponent.generated.h"

#include "GameData/World.h"


ResourceStreamingSystem::ResourceStreamingSystem(WorldHolder& worldHolder, HAL::ResourceManager* resourceManager)
	: mWorldHolder(worldHolder)
	, mResourceManager(resourceManager)
{
}

void ResourceStreamingSystem::update()
{
	World* world = mWorldHolder.world;

	world->getEntityManger().forEachComponentSet<SpriteComponent>([&resourceManager = mResourceManager](SpriteComponent* sprite)
	{
		if (ResourceHandle textureHandle = sprite->getSpriteHandle(); !textureHandle.isValid())
		{
			sprite->setSpriteHandle(resourceManager->lockSprite(sprite->getTexturePath()));
		}
	});
}
