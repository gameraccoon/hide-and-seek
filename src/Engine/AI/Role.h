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

	void update(float deltaTime);
private:
	StatesStack states;
};

#endif