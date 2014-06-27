#include "LuaTypeLog.h"

#include <Debug/Log.h>

// -- lua functions

namespace luafunc
{
	int cmdLogLog(lua_State* state)
	{
		LuaInstance luaInstance(state);

		for(int i = 1; i < luaInstance.getArgumentsCount() + 1; i++)
			Log::WriteLog(luaInstance.getFromLua<char*>(i));
			
		return 0;
	}

	int cmdLogWarning(lua_State* state)
	{
		LuaInstance luaInstance(state);

		for(int i = 1; i < luaInstance.getArgumentsCount() + 1; i++)
			Log::WriteWarning(luaInstance.getFromLua<char*>(i));

		return 0;
	}
	
	int cmdLogError(lua_State* state)
	{
		LuaInstance luaInstance(state);

		for(int i = 1; i < luaInstance.getArgumentsCount() + 1; i++)
			Log::WriteError(luaInstance.getFromLua<char*>(i));

		return 0;
	}
}

// -- end: lua functions

namespace LuaType
{
	void registerLog(LuaInstance *instance, const char* name)
	{
		instance->beginInitializeTable();
		instance->registerTableFunction("log", luafunc::cmdLogLog);
		instance->registerTableFunction("warn", luafunc::cmdLogWarning);
		instance->registerTableFunction("error", luafunc::cmdLogError);
		instance->endInitializeTable(name);
	}
}
