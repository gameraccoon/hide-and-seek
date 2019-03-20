#pragma once

#include <stack>

#include "Role.h"

class PlayerRole : public Role
{
public:
	PlayerRole(World* world, IBody *body);
	virtual ~PlayerRole() override;

	virtual void update(float deltaTime) override;

//	virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse) override;
//	virtual void onSeeEnemy(IActor *enemy) override;
//	virtual void onHearNoise(SoundVolume *sound) override;

    virtual void gotoState(const std::string& stateName) override;
	virtual void endCurrentState() override;
};
