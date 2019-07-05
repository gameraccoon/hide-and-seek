#pragma once

#include <memory>
#include "ECS/System.h"

class TestCircularUnitsSystem : public System
{
public:
	TestCircularUnitsSystem() = default;
	~TestCircularUnitsSystem() override = default;

	void update(World* world, float dt) override;
};
