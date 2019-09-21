#pragma once

#include <memory>
#include <vector>

#include "ECS/System.h"

#include "HAL/Base/ResourceManager.h"
#include "HAL/EngineFwd.h"

#include "GameLogic/SharedManagers/WorldHolder.h"

class Vector2D;

/**
 * System that handles rendering of world objects
 */
class RenderSystem : public System
{
public:
	using KeyStatesMap = std::unordered_map<int, bool>;

public:
	RenderSystem(WorldHolder& worldHolder, HAL::Engine* engine, HAL::ResourceManager* resourceManager);
	~RenderSystem() override = default;

	void update() override;
	std::string getName() override { return "RenderSystem"; }

private:
	void drawVisibilityPolygon(const Graphics::Sprite& lightSprite, const std::vector<Vector2D>& polygon, const Vector2D& fowSize, const Vector2D& drawShift);
	static Vector2D GetPlayerSightPosition(World* world);
	void drawLights(World* world, const Vector2D& drawShift, const Vector2D& maxFov);

private:
	WorldHolder& mWorldHolder;
	HAL::Engine* mEngine;
	HAL::ResourceManager* mResourceManager;
	ResourceHandle mLightSpriteHandle;
};
