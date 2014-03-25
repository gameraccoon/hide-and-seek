#include "LuaType.h"

#include "../Core/IActor.h"

namespace LuaType
{
	template<>
	void registerValue<IActor>(LuaInstance *instance, IActor* value)
	{
		registerField<Vector2D>(instance, "pos", &value->getLocation());
	}
}