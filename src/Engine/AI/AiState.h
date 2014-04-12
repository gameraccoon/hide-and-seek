#ifndef AI_STATE
#define AI_STATE

#include "../Modules/States/State.h"
#include "../Lua/LuaInstance.h"
#include "../Core/World.h"
#include "Role.h"

class AiState : public State
{
public:
	AiState(World *world, IActor *body, Role *role);
	~AiState(void);

	void process(float deltatime);
private:
	LuaInstance *script;

	IActor *body;
	Role *role;

	float scriptUpdateInterval;
	float lastExecutionTime;
};

#endif