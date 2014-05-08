#include "LuaInstance.h"

#include "../Helpers/Log.h"

LuaInstance::LuaInstance()
{
    this->luaState = luaL_newstate();
    
    static const luaL_Reg lualibs[] = 
    {
        {"base", luaopen_base},
        {"io", luaopen_io},
		{"debug", luaopen_debug},
        {NULL, NULL}
    };

	this->registerLibs(lualibs);
}

LuaInstance::LuaInstance(lua_State *luaState)
{
	this->luaState = luaState;
}

LuaInstance::~LuaInstance()
{
    lua_close(this->luaState);
}

lua_State* LuaInstance::getLuaState()
{
	return this->luaState;
}

void LuaInstance::registerLibs(const luaL_Reg lualibs[])
{
    for(const luaL_Reg *lib = lualibs; lib->func != NULL; lib++)
    {
        luaL_requiref(this->luaState, lib->name, lib->func, 1);
        lua_settop(this->luaState, 0);
    }
}

int LuaInstance::execScript(const char* script)
{
	int res = luaL_dostring(this->luaState, script);

	if (res != 0)
	{
		Log::WriteWarning(lua_tostring(this->luaState, -1));
		return res;
	}

	return 0;
}

int LuaInstance::execScriptFromFile(const char* ScriptFileName)
{
	int res = luaL_dofile(this->luaState, ScriptFileName);
	
	if (res != 0)
	{
		Log::WriteWarning(lua_tostring(this->luaState, -1));
		return res;
	}

	return 0;
}

int LuaInstance::getArgumentsCount()
{
    return lua_gettop(this->luaState);
}

template<>
int LuaInstance::getFromLua<int>(int index)
{
    return lua_tointeger(this->luaState, index);
}

template<>
double LuaInstance::getFromLua<double>(int index)
{
    return lua_tonumber(this->luaState, index);
}

template<>
char* LuaInstance::getFromLua<char*>(int index)
{
    return (char*)lua_tostring(this->luaState, index);
}

template<>
const char* LuaInstance::getFromLua<const char*>(int index)
{
    return (const char*)lua_tostring(this->luaState, index);
}

template<>
bool LuaInstance::getFromLua<bool>(int index)
{
    return lua_toboolean(this->luaState, index) != 0;
}


template<>
void LuaInstance::sendToLua<int>(int value)
{
    lua_pushinteger(this->luaState, value);
}

template<>
void LuaInstance::sendToLua<double>(double value)
{
    lua_pushnumber(this->luaState, value);
}

template<>
void LuaInstance::sendToLua<const char*>(const char* value)
{
    lua_pushstring(this->luaState, value);
}

template<>
void LuaInstance::sendToLua<bool>(bool value)
{
    lua_pushboolean(this->luaState, value);
}

template<>
void LuaInstance::sendToLua<lua_CFunction>(lua_CFunction value)
{
	lua_pushcfunction(this->luaState, value);
}

template<>
void LuaInstance::sendToLua<void*>(void* value)
{
	lua_pushlightuserdata(this->luaState, value);
}

void LuaInstance::beginInitializeTable()
{
	lua_newtable(luaState);
}

void LuaInstance::endInitializeTable(const char* tableName)
{
	lua_setglobal(this->luaState, tableName);
}

void LuaInstance::endInitializeSubtable(const char* tableName)
{
	lua_pushstring(this->luaState, "pos");
    lua_insert(this->luaState, -2);
    lua_settable(this->luaState, -3);
}

void LuaInstance::registerFunction(const char* functionName, lua_CFunction function)
{
	this->registerConstant<lua_CFunction>(functionName, function);
}

void LuaInstance::registerTableFunction(const char* functionName, lua_CFunction function)
{
	this->registerTableConstant<const char*, lua_CFunction>(functionName, function);
}

void LuaInstance::removeSymbol(const char* symbolName)
{
	lua_pushnil(this->luaState);
	lua_setglobal(this->luaState, symbolName);
}
