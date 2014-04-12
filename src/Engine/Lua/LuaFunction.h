#ifndef LUA_FUNCTION
#define LUA_FUNCTION

#include "LuaInstance.h"

class LuaFunction
{
public:
	void readyToRunFunction(LuaInstance *instance, const char* functionName);
	void runFunction(int paramsCount, int resultsCount);
	void clearAfterFunction();

	template<typename T>
	void sendParameter(T value);

	template<typename T>
	T getReturnValue(int index);
private:
	lua_State *luaState;
	int stackState;
};

#endif