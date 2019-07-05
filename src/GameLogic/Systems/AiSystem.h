#pragma once

#include <memory>
#include "ECS/System.h"

/**
 * System that calculates AI
 */
class AiSystem : public System
{
public:
	AiSystem() = default;
	~AiSystem() override = default;

	void update(World* world, float dt) override;
};
