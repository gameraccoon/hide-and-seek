#ifndef ROLE_H
#define ROLE_H

#include "../Core/IActor.h"
#include "../Core/World.h"
#include "../Modules/States/StatesStack.h"

class Role
{
public:
	Role(World* world, IActor *body);
	~Role(void);

	virtual void update(float deltaTime);
	IActor* getBody();
private:
	StatesStack states;
	IActor *body;
};

#endif