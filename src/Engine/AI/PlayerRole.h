#ifndef PLAYER_ROLE_H
#define PLAYER_ROLE_H

#include <stack>

#include "Role.h"

class PlayerRole : public Role
{
public:
	PlayerRole(World* world, IBody *body);
	virtual ~PlayerRole(void) override;

	virtual void update(float deltaTime) override;

	virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse) override;
	virtual void onSeeEnemy(IActor *enemy) override;
	virtual void onHearNoise(SoundVolume *sound) override;
};

#endif