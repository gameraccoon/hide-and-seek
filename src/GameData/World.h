#pragma once

#include "ECS/EntityManager.h"
#include "ECS/ComponentSetHolder.h"

class World
{
public:
	OptionalEntity getMainCamera() { return mMainCamera; }
	void setMainCamera(const OptionalEntity& newCamera) { mMainCamera = newCamera; }

	OptionalEntity getPlayerControlledEntity() { return mPlayerControlledEntity; }
	void setPlayerControlledEntity(const OptionalEntity& newEntity) { mPlayerControlledEntity = newEntity; }

	EntityManager& getEntityManager() { return mEntityManager; }
	ComponentSetHolder& getWorldComponents() { return mWorldComponents; }

	nlohmann::json toJson(const ComponentFactory& componentFactory) const;
	void fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory);

private:
	OptionalEntity mMainCamera;
	OptionalEntity mPlayerControlledEntity;

	EntityManager mEntityManager;
	ComponentSetHolder mWorldComponents;
};
