#include "LuaFunction.h"

#include "../Helpers/Log.h"


namespace
{
	int errorHandler(lua_State* luaState)
	{
		//stack: err
		lua_getglobal(luaState, "debug"); // stack: err debug
		lua_getfield(luaState, -1, "traceback"); // stack: err debug debug.traceback

		// debug.traceback() возвращает 1 значение
		if(lua_pcall(luaState, 0, 1, 0))
		{
			const char* err = lua_tostring(luaState, -1);

			Log::WriteWarning(std::string("Error in debug.traceback() call: %s\n").append(err));
		}
		else
		{
			// stack: err debug stackTrace
			//lua_insert(luaState, -2); // stack: err stackTrace debug
			//lua_pop(luaState, 1); // stack: err stackTrace
			//lua_pushstring(luaState, "Error:"); // stack: err stackTrace "Error:"
			//lua_insert(luaState, -3); // stack: "Error:" err stackTrace  
			//lua_pushstring(luaState, "\n"); // stack: "Error:" err stackTrace "\n"
			//lua_insert(luaState, -2); // stack: "Error:" err "\n" stackTrace
			//lua_concat(luaState, 4); // stack: "Error:"+err+"\n"+stackTrace

			const char* stackTrace = lua_tostring(luaState, -1);

			Log::WriteWarning(stackTrace);
		}

		return 1;
	}
}


void LuaFunction::readyToRunFunction(LuaInstance *instance, const char* functionName)
{
	this->luaState = instance->getLuaState();

	lua_getglobal(this->luaState, functionName);
}

void LuaFunction::runFunction(int paramsCount, int resultsCount)
{
	int calculatedResultsCount = resultsCount;

	lua_pushcfunction(this->luaState, errorHandler); // stack: func arg1 arg2 ... argn errorHandler 
	
	const int errorHandlerIndex = -(paramsCount + 2);
	
	lua_insert(this->luaState, errorHandlerIndex); // stack: errorHandler func arg1 arg2 ... argn
	
	int res = lua_pcall(this->luaState, paramsCount, resultsCount, 0);

	if (res != 0)
	{
		calculatedResultsCount = 1; // if error, only one result is placed on stack (instead of N results)
	}
	
	lua_remove(this->luaState, -calculatedResultsCount-1); // remove function errorHandler from stack

	if (res != 0)
	{
		const char* err = lua_tostring(this->luaState, -1);

		Log::WriteWarning(err);
	}
}

void LuaFunction::clearAfterFunction()
{
	lua_pop(this->luaState, 1);
}