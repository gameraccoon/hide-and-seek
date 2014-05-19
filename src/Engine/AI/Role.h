#ifndef ROLE_H
#define ROLE_H

#include "../AI/IBody.h"
#include "../Core/IActor.h"
#include "../Core/World.h"
#include "../AI/AiStatesStack.h"

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
	AiStatesStack states;
	IBody *body;
};

#endif