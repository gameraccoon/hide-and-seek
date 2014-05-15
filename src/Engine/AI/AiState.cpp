#include "AiState.h"

#include "../Lua/LuaType.h"
#include "../Lua/LuaTypeLog.h"
#include "../Lua/LuaFunction.h"


AiState::AiState(World *world, IBody *body, Role *role)
{
	scriptUpdateInterval = 0.01f;
	lastExecutionTime = 0.f;

	this->script = new LuaInstance();

	LuaType::registerLog(this->script, "Log");

	// dummies for events
	this->script->execScript("function OnTakeDamage(instigator, damage) end");

	this->script->execScriptFromFile("test.lua");

	this->body = body;
	this->role = role;
}

AiState::~AiState(void)
{
	delete this->script;
}

/*void AiState::process(float deltatime)
{
	lastExecutionTime += deltatime;

	if (lastExecutionTime > scriptUpdateInterval)
	{
		this->script->registerConstant<double>("deltatime", lastExecutionTime);
		
		LuaType::registerConstant<IBody>(this->script, "body", this->body);

		this->script->removeSymbol("body");

		lastExecutionTime = 0.f;
	}
}*/

void AiState::onTakeDamage(IActor* instigator, float damageValue)
{
	LuaFunction luaFunction;
	luaFunction.readyToRunFunction(this->script, "OnTakeDamage");
	LuaType::registerValue<IActor>(this->script, instigator);
	this->script->sendToLua<double>(10.0);
	luaFunction.runFunction(1, 0);
	luaFunction.clearAfterFunction();
}
