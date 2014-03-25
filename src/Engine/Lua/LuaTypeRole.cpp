#include "LuaType.h"

#include "../AI/Role.h"

namespace LuaType
{
	template<>
	void registerValue<Role>(LuaInstance *instance, Role* value)
	{
		//registerField<Vector2D>(instance, "pos", &value->getLocation());
	}
}