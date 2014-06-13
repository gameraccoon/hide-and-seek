#include "LuaTypeState.h"

#include <Debug/Log.h>

// -- lua functions

namespace luafunc
{
	int cmdGoToState(lua_State* state)
	{
		LuaInstance luaInstance(state);

		AiState *aiState = (AiState*)luaInstance.getFromLua<void*>(1);

		aiState->gotoState(luaInstance.getFromLua<char*>(2));
		
		return 0;
	}

	int cmdEndState(lua_State* state)
	{
		LuaInstance luaInstance(state);

		AiState *aiState = (AiState*)luaInstance.getFromLua<void*>(1);

		aiState->endState();
			
		return 0;
	}
}

// -- end: lua functions

namespace LuaType
{
	void registerStateFunctions(LuaInstance *instance, AiState *state)
	{
		instance->registerFunction("gotoState", luafunc::cmdGoToState);
		instance->registerFunction("endState", luafunc::cmdEndState);
		instance->registerConstant<void*>("statePtr", state);
	}
}
