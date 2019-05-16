#pragma once

#include <memory>
#include <Core/System.h>
#include <Base/ResourceManager.h>
#include <Components/CameraComponent.generated.h>
#include <EngineFwd.h>

#ifdef DEBUG
#include <Graphics/DebugDrawer.h>
#endif // DEBUG

class TransformComponent;
class CollisionComponent;
class Vector2D;

/**
 * System that handles rendering of world objects
 */
class RenderSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	RenderSystem(SystemInterface::Engine* engine, std::shared_ptr<SystemInterface::ResourceManager> resourceManager);
	~RenderSystem() override = default;

	void update(World* world, float dt) override;

private:
	void drawVisibilityPolygon(const std::vector<Vector2D>& polygon, const Vector2D& fowSize, const Vector2D& drawShift);
	static Vector2D GetPlayerSightPosition(World* world);
	void drawLights(World* world, const Vector2D& drawShift, const Vector2D& maxFov);

private:
	SystemInterface::Engine* mEngine;
	std::shared_ptr<SystemInterface::ResourceManager> mResourceManager;

#ifdef DEBUG
	DebugDrawer mDebugDrawer;
#endif // DEBUG
};
