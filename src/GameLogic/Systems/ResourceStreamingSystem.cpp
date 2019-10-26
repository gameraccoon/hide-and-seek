#include "GameLogic/Systems/ResourceStreamingSystem.h"

#include "GameData/Components/SpriteCreatorComponent.generated.h"
#include "GameData/Components/RenderComponent.generated.h"
#include "GameData/Components/AnimationClipsComponent.generated.h"
#include "GameData/Components/AnimationClipCreatorComponent.generated.h"
#include "GameData/Components/AnimationGroupsComponent.generated.h"
#include "GameData/Components/AnimationGroupCreatorComponent.generated.h"

#include "GameData/World.h"
#include "GameData/GameData.h"

#include "HAL/Graphics/Sprite.h"
#include "HAL/Graphics/SpriteAnimationClip.h"
#include "HAL/Graphics/AnimationGroup.h"

ResourceStreamingSystem::ResourceStreamingSystem(WorldHolder& worldHolder, HAL::ResourceManager& resourceManager)
	: mWorldHolder(worldHolder)
	, mResourceManager(resourceManager)
{
}

void ResourceStreamingSystem::update()
{
	World& world = mWorldHolder.getWorld();

	// load sprites
	world.getEntityManager().forEachComponentSetWithEntity<SpriteCreatorComponent>([&resourceManager = mResourceManager, &entityManager = world.getEntityManager()](Entity entity, SpriteCreatorComponent* spriteCreator)
	{
		const auto& descriptions = spriteCreator->getDescriptions();
		Assert(!descriptions.empty(), "Sprite descriptions should not be empty");

		RenderComponent* render = entityManager.scheduleAddComponent<RenderComponent>(entity);
		size_t spritesCount = descriptions.size();
		auto& spriteDatas = render->getSpriteDatasRef();
		spriteDatas.resize(spritesCount);
		for (size_t i = 0; i < spritesCount; ++i)
		{
			spriteDatas[i].spriteHandle = resourceManager.lockSprite(descriptions[i].path);
			spriteDatas[i].params = descriptions[i].params;
			int id = render->getMaxSpriteId();
			render->getSpriteIdsRef().push_back(id++);
			render->setMaxSpriteId(id);
		}
		entityManager.scheduleRemoveComponent<SpriteCreatorComponent>(entity);
	});
	world.getEntityManager().executeScheduledActions();

	// load single animations clips
	world.getEntityManager().forEachComponentSetWithEntity<AnimationClipCreatorComponent>([&resourceManager = mResourceManager, &entityManager = world.getEntityManager()](Entity entity, AnimationClipCreatorComponent* animationClipCreator)
	{
		const auto& descriptions = animationClipCreator->getDescriptionsRef();
		Assert(!descriptions.empty(), "Animation descriptions should not be empty");

		AnimationClipsComponent* animationClips = entityManager.scheduleAddComponent<AnimationClipsComponent>(entity);
		size_t animationCount = descriptions.size();
		auto& animations = animationClips->getDatasRef();
		animations.resize(animationCount);

		auto [render] = entityManager.getEntityComponents<RenderComponent>(entity);
		if (render == nullptr)
		{
			render = entityManager.scheduleAddComponent<RenderComponent>(entity);
		}

		auto& spriteDatas = render->getSpriteDatasRef();
		for (size_t i = 0; i < animationCount; ++i)
		{
			animations[i].animation = resourceManager.lockSpriteAnimationClip(descriptions[i].path);
			animations[i].params = descriptions[i].params;
			animations[i].sprites = resourceManager.getResource<Graphics::SpriteAnimationClip>(animations[i].animation).getSprites();

			AssertFatal(!animations[i].sprites.empty(), "Empty SpriteAnimation '%s'", descriptions[i].path.c_str());
			spriteDatas.emplace_back(descriptions[i].spriteParams, animations[i].sprites.front());
			int id = render->getMaxSpriteId();
			animations[i].spriteId = id;
			render->getSpriteIdsRef().push_back(id++);
			render->setMaxSpriteId(id);

			Assert(render->getSpriteIds().size() == spriteDatas.size(), "Sprites and SpriteIds have diverged");
		}

		entityManager.scheduleAddComponent<AnimationGroupsComponent>(entity);

		entityManager.scheduleRemoveComponent<AnimationClipCreatorComponent>(entity);
	});
	world.getEntityManager().executeScheduledActions();

	// load animation groups
	world.getEntityManager().forEachComponentSetWithEntity<AnimationGroupCreatorComponent>([&resourceManager = mResourceManager, &entityManager = world.getEntityManager()](Entity entity, AnimationGroupCreatorComponent* animationGroupCreator)
	{
		AnimationGroupsComponent* animationGroups = entityManager.scheduleAddComponent<AnimationGroupsComponent>(entity);

		auto [animationClips] = entityManager.getEntityComponents<AnimationClipsComponent>(entity);
		if (animationClips == nullptr)
		{
			animationClips = entityManager.scheduleAddComponent<AnimationClipsComponent>(entity);
		}
		auto& clipDatas = animationClips->getDatasRef();

		auto [render] = entityManager.getEntityComponents<RenderComponent>(entity);
		if (render == nullptr)
		{
			render = entityManager.scheduleAddComponent<RenderComponent>(entity);
		}
		auto& spriteDatas = render->getSpriteDatasRef();

		size_t i = 0;
		for (const std::string& groupPath : animationGroupCreator->getAnimationGroups())
		{
			ResourceHandle animGroupHandle = resourceManager.lockAnimationGroup(groupPath);
			const Graphics::AnimationGroup& group = resourceManager.getResource<Graphics::AnimationGroup>(animGroupHandle);
			AnimationGroup<std::string> animationGroup;
			animationGroup.currentState = group.getDefaultState();
			animationGroup.animationClips = group.getAnimationClips();
			animationGroup.stateMachineId = group.getStateMachineID();
			animationGroup.animationClipIdx = clipDatas.size();

			AnimationClip clip;
			clip.params = animationGroupCreator->getClipParams()[i];
			clip.sprites = animationGroup.animationClips.find(animationGroup.currentState)->second;

			animationGroups->getDataRef().emplace_back(std::move(animationGroup));

			spriteDatas.emplace_back(animationGroupCreator->getSpriteParams()[i], clip.sprites.front());

			int id = render->getMaxSpriteId();
			clip.spriteId = id;
			render->getSpriteIdsRef().push_back(id++);
			render->setMaxSpriteId(id);

			clipDatas.emplace_back(std::move(clip));

			++i;
		}

		entityManager.scheduleRemoveComponent<AnimationGroupCreatorComponent>(entity);
	});
	world.getEntityManager().executeScheduledActions();
}
