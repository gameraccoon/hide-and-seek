#include "AiState.h"

#include "../Helpers/Log.h"

AiState::AiState(World *world, IActor *body)
{
	this->script = world->getLuaInstance();
}

AiState::~AiState(void)
{
	Log::WriteLog("Destruct AiState");
}

void AiState::process()
{
	this->script->execScriptFromFile("test.lua");
}
