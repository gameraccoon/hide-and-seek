#include "AiState.h"

#include "../Helpers/Log.h"
#include "../Lua/LuaType.h"

AiState::AiState(World *world, IActor *body, Role *role)
{
	this->script = world->getLuaInstance();
	scriptUpdateInterval = 10.f;
	lastExecutionTime = 0.f;

	this->body = body;
	this->role = role;
}

AiState::~AiState(void)
{
}

void AiState::process(float deltatime)
{

	lastExecutionTime += deltatime;

	if (lastExecutionTime > scriptUpdateInterval)
	{
		lastExecutionTime = 0.f;

		this->script->registerConstant<double>("deltatime", deltatime);
		
		LuaType::registerConstant<IActor>(this->script, "body", this->body);

		this->script->execScriptFromFile("test.lua");

		//this->script->removeSymbol("this");
		this->script->removeSymbol("body");
	}
}
