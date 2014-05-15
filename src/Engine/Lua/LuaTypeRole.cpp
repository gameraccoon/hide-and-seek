#include "LuaType.h"

#include "../AI/Role.h"
#include "../AI/IBody.h"

namespace LuaType
{
	template<>
	void registerValue<Role>(LuaInstance *instance, Role* value)
	{
		registerField<IBody>(instance, "body", value->getBody());
	}
}