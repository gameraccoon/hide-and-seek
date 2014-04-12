#include "AiState.h"

#include "../Helpers/Log.h"
#include "../Lua/LuaType.h"
#include "../Lua/LuaTypeLog.h"
#include "../Lua/LuaFunction.h"

AiState::AiState(World *world, IActor *body, Role *role)
{
	scriptUpdateInterval = 0.01f;
	lastExecutionTime = 0.f;

	this->script = new LuaInstance();

	LuaType::registerLog(this->script, "Log");
	this->script->execScriptFromFile("test.lua");

	this->body = body;
	this->role = role;
}

AiState::~AiState(void)
{
	delete this->script;
}

void AiState::process(float deltatime)
{
	lastExecutionTime += deltatime;

	if (lastExecutionTime > scriptUpdateInterval)
	{
		this->script->registerConstant<double>("deltatime", lastExecutionTime);
		
		//LuaType::registerConstant<IActor>(&this->script, "body", this->body);
		
		LuaFunction luaFunction;
		luaFunction.readyToRunFunction(this->script, "OnTakeDamage");
		//this->script->sendToLua<double>(10.0);
		luaFunction.runFunction(0, 1);
		int z1 = (int)this->script->getFromLua<double>(-1);
		luaFunction.clearAfterFunction();

		//this->script.removeSymbol("body");

		lastExecutionTime = 0.f;
	}
}
