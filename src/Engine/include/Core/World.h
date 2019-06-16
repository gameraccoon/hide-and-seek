#pragma once

#include <Core/EntityManager.h>
#include <Core/ComponentSetHolder.h>

class World
{
public:
	NullableEntity getMainCamera() { return mMainCamera; }
	void setMainCamera(const NullableEntity& newCamera) { mMainCamera = newCamera; }

	NullableEntity getPlayerControlledEntity() { return mPlayerControlledEntity; }
	void setPlayerControlledEntity(const NullableEntity& newEntity) { mPlayerControlledEntity = newEntity; }

	EntityManager& getEntityManger() { return mEntityManager; }
	ComponentSetHolder& getWorldComponents() { return mWorldComponents; }

	nlohmann::json toJson(const ComponentFactory& componentFactory) const;
	void fromJson(const nlohmann::json& json, const ComponentFactory& componentFactory);

private:
	NullableEntity mMainCamera;
	NullableEntity mPlayerControlledEntity;

	EntityManager mEntityManager;
	ComponentSetHolder mWorldComponents;
};
