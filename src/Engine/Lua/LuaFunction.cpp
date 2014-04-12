#include "LuaFunction.h"

#include "../Helpers/Log.h"


namespace
{
	int errorHandler(lua_State* luaState)
	{	
		lua_getglobal(luaState, "debug");
		lua_getfield(luaState, -1, "traceback");

		// call debug.traceback()
		if(lua_pcall(luaState, 0, 1, 0))
		{
			const char* err = lua_tostring(luaState, -1);

			Log::WriteWarning(std::string("\nError in debug.traceback() call: %s\n").append(err));
		}
		else
		{
			// concatenating the error message and trace of the stack
			lua_insert(luaState, -2);
			lua_pop(luaState, 1);
			lua_pushstring(luaState, "\n");
			lua_insert(luaState, -2);
			lua_concat(luaState, 3);
			// then return concatinated value
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
	
	// pushing errorHandler to the head of stack
	lua_pushcfunction(this->luaState, errorHandler);
	// calculating new errorHandler's position in the stack
	const int errorHandlerIndex = -(paramsCount + 2);
	// change errorHandler's position in the stack
	lua_insert(this->luaState, errorHandlerIndex);
	// call function
	int res = lua_pcall(this->luaState, paramsCount, resultsCount, errorHandlerIndex);

	// if there is error then we have single result
	if (res != 0)
	{
		calculatedResultsCount = 1;
	}

	// remove function errorHandler from stack
	lua_remove(this->luaState, -calculatedResultsCount-1);

	if (res != 0)
	{
		// this message returned by errorHandler
		const char* err = lua_tostring(this->luaState, -1);

		Log::WriteWarning(err);
	}
}

void LuaFunction::clearAfterFunction()
{
	lua_pop(this->luaState, 1);
}