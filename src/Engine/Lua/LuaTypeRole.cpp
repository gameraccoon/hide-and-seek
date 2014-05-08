#include "LuaType.h"

#include "../AI/Role.h"
#include "../Core/Vector2D.h"

namespace LuaType
{
	template<>
	void registerValue<Role>(LuaInstance *instance, Role* value)
	{
		registerField<Vector2D>(instance, "body", &value->getBody()->getLocation());
	}
}