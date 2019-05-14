#pragma once

#include <unordered_map>

#include <Core/System.h>
#include <Base/Engine.h>

class World;

/**
 * System that resolve object collisions
 */
class CollisionSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	~CollisionSystem() override = default;

	void update(World* world, float dt) override;
};
