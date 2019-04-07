#pragma once

#include <Core/EntityManager.h>

class World
{
public:
	NullableEntity getMainCamera() { return mMainCamera; }
	void setMainCamera(const NullableEntity& newCamera) { mMainCamera = newCamera; }

	NullableEntity getPlayerControlledEntity() { return mPlayerControlledEntity; }
	void setPlayerControlledEntity(const NullableEntity& newEntity) { mPlayerControlledEntity = newEntity; }

	EntityManager& getEntityManger() { return mEntityManager; }

	friend void to_json(nlohmann::json& outJson, const World& world);
	friend void from_json(const nlohmann::json& json, World& outWorld);

private:
	NullableEntity mMainCamera;
	NullableEntity mPlayerControlledEntity;

	EntityManager mEntityManager;
};
