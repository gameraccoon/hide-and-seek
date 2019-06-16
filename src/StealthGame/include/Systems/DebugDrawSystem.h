#pragma once

#include <memory>
#include <Core/System.h>
#include <Base/ResourceManager.h>
#include <Components/CameraComponent.generated.h>
#include <EngineFwd.h>

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
	DebugDrawSystem(SystemInterface::Engine* engine, const std::shared_ptr<SystemInterface::ResourceManager>& resourceManager);
	~DebugDrawSystem() override = default;

	void update(World* world, float dt) override;

private:
	void drawVisibilityPolygon(const std::vector<Vector2D>& polygon, const Vector2D& fowSize, const Vector2D& drawShift);
	static Vector2D GetPlayerSightPosition(World* world);
	void drawLights(World* world, const Vector2D& drawShift, const Vector2D& maxFov);

private:
	SystemInterface::Engine* mEngine;
	std::shared_ptr<SystemInterface::ResourceManager> mResourceManager;

	ResourceHandle mCollisionTextureHandle;
	ResourceHandle mNavmeshTextureHandle;
};
