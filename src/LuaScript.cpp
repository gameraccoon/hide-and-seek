#include "../src/LuaScript.h"

using namespace lua;

LuaScript::LuaScript()
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

LuaScript::~LuaScript()
{
    lua_close(this->luaState);
}

int LuaScript::execScript(const char* script)
{
	luaL_dostring(this->luaState, script);

    return lua_tointeger(this->luaState, lua_gettop(this->luaState));
}

int LuaScript::execScriptFromFile(const char* ScriptFileName)
{
    luaL_dofile(this->luaState, ScriptFileName);

    return lua_tointeger(this->luaState, lua_gettop(this->luaState));
}

int LuaScript::getArgumentsCount()
{
    return lua_gettop(this->luaState);
}

template<>
int LuaScript::getArgument<int>(int index)
{
    return lua_tointeger(this->luaState, index);
}

template<>
double LuaScript::getArgument<double>(int index)
{
    return lua_tonumber(this->luaState, index);
}

template<>
char* LuaScript::getArgument<char*>(int index)
{
    return (char*)lua_tostring(this->luaState, index);
}

template<>
bool LuaScript::getArgument<bool>(int index)
{
    return lua_toboolean(this->luaState, index) != 0;
}


template<>
void LuaScript::sendToLua<int>(int value)
{
    lua_pushinteger(this->luaState, value);
}

template<>
void LuaScript::sendToLua<double>(double value)
{
    lua_pushnumber(this->luaState, value);
}

template<>
void LuaScript::sendToLua<char*>(char* value)
{
    lua_pushstring(this->luaState, value);
}

template<>
void LuaScript::sendToLua<bool>(bool value)
{
    lua_pushboolean(this->luaState, value);
}

template<>
void LuaScript::sendToLua<lua_CFunction>(lua_CFunction value)
{
	lua_pushcfunction(this->luaState, value);
}

void LuaScript::beginInitializeTable()
{
	lua_createtable(luaState, 2, 0);
	this->luaState = luaState;
}

void LuaScript::endInitializeTable(const char* arrayName)
{
	lua_setglobal(this->luaState, arrayName);
}

void LuaScript::registerFunction(const char* functionName, lua_CFunction function)
{
	this->registerConstant<lua_CFunction>(functionName, function);
}