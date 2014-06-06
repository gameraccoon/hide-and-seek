#include "LuaType.h"

#include <Engine/AI/Role.h>
#include <Engine/AI/IBody.h>

namespace LuaType
{
	template<>
	void registerValue<Role>(LuaInstance *instance, Role* value)
	{
		registerField<IBody>(instance, "body", value->getBody());
	}
}