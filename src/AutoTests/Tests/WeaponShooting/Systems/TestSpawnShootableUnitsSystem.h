#pragma once

#include <unordered_map>

#include "ECS/System.h"

#include "GameData/Core/Vector2D.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

class TestSpawnShootableUnitsSystem : public System
{
public:
	TestSpawnShootableUnitsSystem(WorldHolder& worldHolder);

	void update() override;
	std::string getName() override { return "TestSpawnShootableUnitsSystem"; }

private:
	static void spawnUnit(class EntityManager& entityManager, Vector2D pos);
	static void spawnJitteredUnit(const Vector2D& pos, const Vector2D& centerShifted, class SpatialWorldData& spatialData);
	static void spawnUnits(class SpatialWorldData& spatialData, int count, Vector2D pos);

private:
	WorldHolder& mWorldHolder;
	int ticksPassed = 0;

	static inline const int jitterRand = 500;
	static inline const float jitterMax = 30.0f;
	static inline const float halfJitterMax = jitterMax / 2.0f;
	static inline const float jitterDivider = jitterRand / jitterMax;
	static inline const float distance = 50.0f;
};
