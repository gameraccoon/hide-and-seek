#include "LuaType.h"
#include "../Core/Vector2D.h"

namespace LuaType
{
	template<>
	void registerValue<Vector2D>(LuaInstance *instance, Vector2D* value)
	{
		instance->registerTableConstant<const char*, double>("x", value->x);
		instance->registerTableConstant<const char*, double>("y", value->y);
	}
}