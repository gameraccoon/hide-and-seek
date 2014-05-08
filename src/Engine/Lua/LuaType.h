#ifndef LUA_TYPE_H
#define LUA_TYPE_H

#include "LuaInstance.h"

namespace LuaType
{
	template<typename T>
	void registerConstant(LuaInstance *instance, const char* name, T* value);
	
	template<typename T>
	void registerField(LuaInstance *instance, const char* name, T* value);

	template<typename T>
	void startRegistration(LuaInstance *instance, T* value);

	template<typename T>
	void registerValue(LuaInstance *instance, T* value);

	template<typename T>
	void registerConstant(LuaInstance *instance, const char* name, T* value)
	{
		instance->beginInitializeTable();
		registerValue<T>(instance, value);
		instance->endInitializeTable(name);
	}

	template<typename T>
	void registerField<T>(LuaInstance *instance, const char* name, T* value)
	{
		instance->beginInitializeTable();
		registerValue<T>(instance, value);
		instance->endInitializeSubtable(name);
	}
}

#endif