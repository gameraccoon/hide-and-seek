#pragma once

#include <unordered_map>

#include "ECS/System.h"

class TestUnitsCountControlSystem : public System
{
public:
	void update(World* world, float dt) override;

private:
	int ticksPassed = 0;
};
