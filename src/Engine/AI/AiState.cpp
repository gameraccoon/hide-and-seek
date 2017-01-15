#include "AiState.h"

AiState::AiState(World *world, IBody *body, Role *role)
{
	mLastExecutionTime = 0.f;

	body = body;
	role = role;
	world = world;

	mEnded = false;
}

AiState::~AiState()
{
}

void AiState::gotoState(std::string stateName)
{
	mRole->gotoState(stateName);
}

void AiState::endState()
{
	mRole->endCurrentState();
}

bool AiState::isEnded() const
{
	return mEnded;
}

void AiState::setEnded(bool ended)
{
	mEnded = ended;
}
