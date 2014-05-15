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

	virtual void onTakeDamage(IActor* instigator, float damageValue);

	IBody* getBody();
private:
	AiStatesStack states;
	IBody *body;
};

#endif