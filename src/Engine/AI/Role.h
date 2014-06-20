#ifndef ROLE_H
#define ROLE_H

#include <stack>

#include "../AI/IBody.h"
#include "../Core/IActor.h"
#include "../Core/World.h"
#include "IAiState.h"

class Role
{
public:
	Role(World* world, IBody *body);
	virtual ~Role(void);

	virtual void update(float deltaTime) = 0;

	virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse) = 0;
	virtual void onSeeEnemy(IActor *enemy) = 0;
	virtual void onHearNoise(SoundVolume *sound) = 0;

	virtual void gotoState(std::string stateName) = 0;
	virtual void endCurrentState() = 0;

	IBody* getBody() const;
	World* getWorld() const;
private:
	IBody *body;
	World *world;
};

#endif