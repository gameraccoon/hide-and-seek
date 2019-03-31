#pragma once

#include <Core/System.h>
#include <Base/Engine.h>

/**
 * System that resolve object collisions
 */
class CollisionSystem : public System
{
public:
	typedef std::unordered_map<int, bool> KeyStatesMap;

public:
	virtual ~CollisionSystem() override = default;

	virtual void update(World* world, float dt) override;
};
