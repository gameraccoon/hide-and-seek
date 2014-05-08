#include "LuaType.h"
#include "../Helpers/Log.h"

// -- lua functions

namespace luafunc
{
	int cmdLogLog(lua_State* state)
	{
		LuaInstance *luaInstance = new LuaInstance(state);

		for(int i = 1; i < luaInstance->getArgumentsCount() + 1; i++)
			Log::WriteLog(luaInstance->getFromLua<char*>(i));
			
		return 0;
	}

	int cmdLogWarning(lua_State* state)
	{
		LuaInstance *luaInstance = new LuaInstance(state);

		for(int i = 1; i < luaInstance->getArgumentsCount() + 1; i++)
			Log::WriteWarning(luaInstance->getFromLua<char*>(i));

		return 0;
	}
	
	int cmdLogError(lua_State* state)
	{
		LuaInstance *luaInstance = new LuaInstance(state);

		for(int i = 1; i < luaInstance->getArgumentsCount() + 1; i++)
			Log::WriteError(luaInstance->getFromLua<char*>(i));

		return 0;
	}
}

// -- end: lua functions

namespace LuaType
{
	void registerLog(LuaInstance *instance, const char* name)
	{
		instance->beginInitializeTable();
		instance->registerTableFunction("Log", luafunc::cmdLogLog);
		instance->registerTableFunction("Warn", luafunc::cmdLogWarning);
		instance->registerTableFunction("Error", luafunc::cmdLogError);
		instance->endInitializeTable(name);
	}
}
