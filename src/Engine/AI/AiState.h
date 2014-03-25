#pragma once

#include "../Modules/States/State.h"
#include "../Lua/LuaInstance.h"
#include "../Core/World.h"

class AiState : public State
{
public:
	AiState(World *world, IActor *body);
	~AiState(void);

	void process();
private:
	LuaInstance *script;
};

