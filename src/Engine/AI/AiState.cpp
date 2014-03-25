#include "AiState.h"

AiState::AiState(World *world, IActor *body)
{
	this->script = world->getLuaInstance();
}

AiState::~AiState(void)
{

}

void AiState::process()
{
	this->script->execScript("Log('asd')");
}
