#include "LuaType.h"

#include "../AI/IBody.h"

namespace LuaType
{
	int cmdMoveTo(lua_State* luaState)
	{
		IBody* body = (IBody*) lua_touserdata(luaState, 1);
		body->moveTo(Vector2D(0.0, 5.5));
		return 0;
	}

	int cmdFollow(lua_State* luaState)
	{
		IBody* body = (IBody*) lua_touserdata(luaState, 1);
		IActor* target = (IActor*) lua_touserdata(luaState, 2);
		body->follow(target);
		return 0;
	}

	template<>
	void registerValue<IBody>(LuaInstance *instance, IBody* value)
	{
		instance->registerTableConstant<const char*, void*>("ptr", value);
		instance->registerTableFunction("moveTo", LuaType::cmdMoveTo);
		instance->registerTableFunction("follow", LuaType::cmdFollow);
	}
}