#include "LuaInstance.h"

LuaInstance::LuaInstance()
{
    this->luaState = luaL_newstate();
    
    static const luaL_Reg lualibs[] = 
    {
        {"base", luaopen_base},
        {"io", luaopen_io},
        {NULL, NULL}
    };

    for(const luaL_Reg *lib = lualibs; lib->func != NULL; lib++)
    {
        luaL_requiref(luaState, lib->name, lib->func, 1);
        lua_settop(luaState, 0);
    }
}

LuaInstance::LuaInstance(lua_State *luaState)
{
	this->luaState = luaState;
}

LuaInstance::~LuaInstance()
{
    lua_close(this->luaState);
}

int LuaInstance::execScript(const char* script)
{
	luaL_dostring(this->luaState, script);

    return lua_tointeger(this->luaState, lua_gettop(this->luaState));
}

int LuaInstance::execScriptFromFile(const char* ScriptFileName)
{
    luaL_dofile(this->luaState, ScriptFileName);

    return lua_tointeger(this->luaState, lua_gettop(this->luaState));
}

int LuaInstance::getArgumentsCount()
{
    return lua_gettop(this->luaState);
}

template<>
int LuaInstance::getArgument<int>(int index)
{
    return lua_tointeger(this->luaState, index);
}

template<>
double LuaInstance::getArgument<double>(int index)
{
    return lua_tonumber(this->luaState, index);
}

template<>
char* LuaInstance::getArgument<char*>(int index)
{
    return (char*)lua_tostring(this->luaState, index);
}

template<>
bool LuaInstance::getArgument<bool>(int index)
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

void LuaInstance::beginInitializeTable()
{
	lua_createtable(luaState, 2, 0);
	this->luaState = luaState;
}

void LuaInstance::endInitializeTable(const char* arrayName)
{
	lua_setglobal(this->luaState, arrayName);
}

void LuaInstance::registerFunction(const char* functionName, lua_CFunction function)
{
	this->registerConstant<lua_CFunction>(functionName, function);
}

void LuaInstance::registerTableFunction(const char* functionName, lua_CFunction function)
{
	this->registerTableConstant<const char*, lua_CFunction>(functionName, function);
}