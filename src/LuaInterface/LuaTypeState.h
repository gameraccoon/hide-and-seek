#ifndef LUA_TYPE_STATE_H
#define LUA_TYPE_STATE_H

#include <Engine/AI/AiState.h>

#include "LuaType.h"

namespace LuaType
{
	void registerStateFunctions(LuaInstance *instance, AiState *state);
}

#endif