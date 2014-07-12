#include "LuaType.h"

#include <Engine/AI/IBody.h>

template<>
IBody* LuaInstance::getFromLua<IBody*>(int index)
{
	return (IBody*) lua_touserdata(this->luaState, index);
}

namespace LuaType
{
	int cmdMoveTo(lua_State* luaState)
	{
		LuaInstance instance(luaState);
		IBody* body = instance.getFromLua<IBody*>(1);
		IActor* target = instance.getFromLua<IActor*>(2);

		body->moveTo(Vector2D(target->getLocation()));

		return 0;
	}

	int cmdFollow(lua_State* luaState)
	{
		LuaInstance instance(luaState);
		IBody* body = instance.getFromLua<IBody*>(1);
		IActor* target = instance.getFromLua<IActor*>(2);
		
		body->follow(target);

		return 0;
	}

	int cmdGetHealth(lua_State* luaState)
	{
		LuaInstance instance(luaState);
		IBody* body = instance.getFromLua<IBody*>(1);

		instance.sendToLua<double>(body->getHealthValue());
		return 1;
	}

	int cmdStartShoting(lua_State* luaState)
	{
		LuaInstance instance(luaState);
		IBody* body = instance.getFromLua<IBody*>(1);
		IActor* target = instance.getFromLua<IActor*>(2);

		body->startShoting(target->getLocation());

		return 0;
	}

	int cmdStopShoting(lua_State* luaState)
	{
		LuaInstance instance(luaState);
		IBody* body = instance.getFromLua<IBody*>(1);

		body->stopShoting();

		return 0;
	}

	template<>
	void registerValue<IBody>(LuaInstance *instance, IBody* value)
	{
		instance->registerTableConstant<const char*, void*>("ptr", value);
		instance->registerTableFunction("moveTo", LuaType::cmdMoveTo);
		instance->registerTableFunction("follow", LuaType::cmdFollow);
		instance->registerTableFunction("getHealth", LuaType::cmdGetHealth);
		instance->registerTableFunction("startShoting", LuaType::cmdStartShoting);
		instance->registerTableFunction("stopShoting", LuaType::cmdStopShoting);
	}
}