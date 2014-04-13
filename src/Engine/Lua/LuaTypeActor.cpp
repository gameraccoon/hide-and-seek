#include "LuaType.h"

#include "../Core/IActor.h"
#include "../Modules/WorldsContainer.h"

#include "../Helpers/Log.h"

namespace LuaType
{
	int cmdGetRotation(lua_State* luaState)
	{
		IActor* actor = (IActor*)lua_touserdata(luaState, 1);
		lua_pushnumber(luaState, actor->getRotation().getValue());
		return 1;
	}

	int cmdSetRotation(lua_State* luaState)
	{
		IActor* actor = (IActor*) lua_touserdata(luaState, 1);
		actor->setRotation(lua_tonumber(luaState, 2));
		return 0;
	}

	template<>
	void registerValue<IActor>(LuaInstance *instance, IActor* value)
	{
		instance->registerTableConstant<const char*, void*>("ptr", value);
		registerField<Vector2D>(instance, "pos", &value->getLocation());
		instance->registerTableConstant<const char*, const char*>("id", value->getActorId().c_str());
		instance->registerTableFunction("getRotation", LuaType::cmdGetRotation);
		instance->registerTableFunction("setRotation", LuaType::cmdSetRotation);
	}
}