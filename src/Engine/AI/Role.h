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

	virtual void update(float deltaTime);

	virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse);
	virtual void onSeeEnemy(IActor *enemy);
	virtual void onHearNoise(SoundVolume *sound);

	IBody* getBody();
private:
	std::stack<IAiState*> states;
	IBody *body;
};

#endif