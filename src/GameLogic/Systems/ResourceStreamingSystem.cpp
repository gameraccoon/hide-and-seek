#include "GameLogic/Systems/ResourceStreamingSystem.h"

#include "GameData/Components/SpriteCreatorComponent.generated.h"
#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/AnimationDataComponent.generated.h"
#include "GameData/Components/AnimationCreatorComponent.generated.h"

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
//	GameData* gameData = mWorldHolder.gameData;

	world->getEntityManager().forEachComponentSetWithEntity<SpriteCreatorComponent>([&resourceManager = mResourceManager, &entityManager = world->getEntityManager()](Entity entity, SpriteCreatorComponent* spriteCreator)
	{
		const auto& descriptions = spriteCreator->getDescriptions();
		Assert(!descriptions.empty(), "Sprite descriptions should not be empty");

		RenderComponent* render = entityManager.scheduleAddComponent<RenderComponent>(entity);
		size_t spritesCount = descriptions.size();
		auto& spriteDatas = render->getSpriteDatasRef();
		spriteDatas.resize(spritesCount);
		for (size_t i = 0; i < spritesCount; ++i)
		{
			spriteDatas[i].spriteHandle = resourceManager->lockSprite(descriptions[i].path);
			spriteDatas[i].params = descriptions[i].params;
			int id = render->getMaxSpriteId();
			render->getSpriteIdsRef().push_back(id++);
			render->setMaxSpriteId(id);
		}
		entityManager.scheduleRemoveComponent<SpriteCreatorComponent>(entity);
	});
	world->getEntityManager().executeScheduledActions();

	world->getEntityManager().forEachComponentSetWithEntity<AnimationCreatorComponent>([&resourceManager = mResourceManager, &entityManager = world->getEntityManager()](Entity entity, AnimationCreatorComponent* animationCreator)
	{
		const auto& descriptions = animationCreator->getDescriptionsRef();
		Assert(!descriptions.empty(), "Animation descriptions should not be empty");

		AnimationDataComponent* animationData = entityManager.scheduleAddComponent<AnimationDataComponent>(entity);
		size_t animationCount = descriptions.size();
		auto& animations = animationData->getDatasRef();
		animations.resize(animationCount);

		auto [render] = entityManager.getEntityComponents<RenderComponent>(entity);
		if (render == nullptr)
		{
			render = entityManager.scheduleAddComponent<RenderComponent>(entity);
		}

		auto& spriteDatas = render->getSpriteDatasRef();
		for (size_t i = 0; i < animationCount; ++i)
		{
			animations[i].animation = resourceManager->lockSpriteAnimation(descriptions[i].path);
			animations[i].params = descriptions[i].params;
			animations[i].sprites = resourceManager->getResource<Graphics::SpriteAnimation>(animations[i].animation).getSprites();

			AssertFatal(!animations[i].sprites.empty(), "Empty SpriteAnimation '%s'", descriptions[i].path);
			spriteDatas.emplace_back(descriptions[i].spriteParams, animations[i].sprites.front());
			int id = render->getMaxSpriteId();
			render->getSpriteIdsRef().push_back(id++);
			render->setMaxSpriteId(id);

			Assert(render->getSpriteIds().size() == spriteDatas.size(), "Sprites and SpriteIds have diverged");
		}

		entityManager.scheduleRemoveComponent<AnimationCreatorComponent>(entity);
	});
	world->getEntityManager().executeScheduledActions();
}
