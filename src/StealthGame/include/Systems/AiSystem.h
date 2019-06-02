#pragma once

#include <memory>
#include <Core/System.h>

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
