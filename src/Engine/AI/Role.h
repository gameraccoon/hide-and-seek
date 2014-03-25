#ifndef ROLE_H
#define ROLE_H

#include "../Core/IActor.h"
#include "../Modules/States/StatesStack.h"

class Role
{
public:
	Role(IActor *body);
	~Role(void);

	void Update(float deltaTime);
private:
	StatesStack states;
};

#endif