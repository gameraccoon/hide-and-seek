#include "AI/AiState.h"

AiState::AiState(World *world, IBody *body, Role *role)
    : mBody(body)
    , mRole(role)
    , mWorld(world)
{
	mLastExecutionTime = 0.f;

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
