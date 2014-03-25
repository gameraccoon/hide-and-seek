#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

extern "C"
{
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

class LuaInstance
{
private:
	lua_State *luaState;

public:
	LuaInstance();
	virtual ~LuaInstance();
		
	int execScript(const char* script);
	int execScriptFromFile(const char* scriptFileName);
		
	int getArgumentsCount();

	template<typename T>
	void registerConstant(const char* constantname, T value);
	void registerFunction(const char* functionName, lua_CFunction function);

	template<typename T>
	T getArgument(int index);

	template<typename T>
	void sendToLua(T value);

	void beginInitializeTable();
	template<typename T1, typename T2>
	void registerTableConstant(T1 key, T2 value);
	void endInitializeTable(const char* arrayName);
};

template<typename T>
void LuaInstance::registerConstant(const char* constantName, T value)
{
	this->sendToLua<T>(value);
	lua_setglobal(this->luaState, constantName);
}

template<typename T1, typename T2>
void LuaInstance::registerTableConstant(T1 key, T2 value)
{
	this->sendToLua<T1>(key);
	this->sendToLua<T2>(value);
	lua_settable(this->luaState, -3);
}

#endif