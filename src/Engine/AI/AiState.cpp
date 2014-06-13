#include "AiState.h"

AiState::AiState(World *world, IBody *body, Role *role)
{
	lastExecutionTime = 0.f;

	this->body = body;
	this->role = role;
}

AiState::~AiState(void)
{
}

void AiState::gotoState(std::string stateName)
{
	this->role->gotoState(stateName);
}

void AiState::endState()
{
	this->role->endCurrentState();
}