#pragma once

#include <memory>
#include "ECS/System.h"
#include "HAL/Base/ResourceManager.h"
#include "GameData/Components/CameraComponent.generated.h"
#include "HAL/EngineFwd.h"

class TransformComponent;
class CollisionComponent;
class Vector2D;

/**
 * System that handles rendering of world objects
 */
class DebugDrawSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	DebugDrawSystem(HAL::Engine* engine, const std::shared_ptr<HAL::ResourceManager>& resourceManager);
	~DebugDrawSystem() override = default;

	void update(World* world, float dt) override;

private:
	void drawVisibilityPolygon(const std::vector<Vector2D>& polygon, const Vector2D& fowSize, const Vector2D& drawShift);
	static Vector2D GetPlayerSightPosition(World* world);
	void drawLights(World* world, const Vector2D& drawShift, const Vector2D& maxFov);

private:
	HAL::Engine* mEngine;
	std::shared_ptr<HAL::ResourceManager> mResourceManager;

	ResourceHandle mCollisionTextureHandle;
	ResourceHandle mNavmeshTextureHandle;
};