#include "GameLogic/Systems/CharacterStateSystem.h"

#include "Debug/Log.h"

#include "GameData/Components/CharacterStateComponent.generated.h"
#include "GameData/Components/StateMachineComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"

#include "GameData/World.h"
#include "GameData/GameData.h"


CharacterStateSystem::CharacterStateSystem(WorldHolder &worldHolder)
	: mWorldHolder(worldHolder)
{
}

static bool CanMove(CharacterState /*state*/)
{
	return true;
}

static bool IsRunning(CharacterState state)
{
	return state == CharacterState::Run;
}

void CharacterStateSystem::update()
{
	World* world = mWorldHolder.world;
	GameData* gameData = mWorldHolder.gameData;

	auto [stateMachine] = gameData->getGameComponents().getComponents<StateMachineComponent>();

	if (stateMachine)
	{
		world->getEntityManger().forEachComponentSet<CharacterStateComponent, MovementComponent>([stateMachine](CharacterStateComponent* characterState, MovementComponent* movement)
		{
			CharacterState state = stateMachine->getCharacterSM().getNextState(characterState->getBlackboard(), characterState->getState());
			characterState->setState(state);

			if (!CanMove(state))
			{
				movement->setMoveDirection(ZERO_VECTOR);
				movement->setSightDirection(ZERO_VECTOR);
			}

			movement->setSpeed(IsRunning(state) ? movement->getOriginalSpeed() * 2.0f : movement->getOriginalSpeed());
		});
	}
}
