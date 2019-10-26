#include "GameLogic/Systems/CharacterStateSystem.h"

#include "Debug/Log.h"

#include "GameData/Components/CharacterStateComponent.generated.h"
#include "GameData/Components/StateMachineComponent.generated.h"
#include "GameData/Components/MovementComponent.generated.h"
#include "GameData/Components/AnimationGroupsComponent.generated.h"

#include "GameData/World.h"
#include "GameData/GameData.h"


CharacterStateSystem::CharacterStateSystem(WorldHolder& worldHolder)
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
	World& world = mWorldHolder.getWorld();
	GameData& gameData = mWorldHolder.getGameData();

	auto [stateMachine] = gameData.getGameComponents().getComponents<StateMachineComponent>();

	if (stateMachine)
	{
		world.getEntityManager().forEachComponentSet<CharacterStateComponent, MovementComponent, AnimationGroupsComponent>([stateMachine](CharacterStateComponent* characterState, MovementComponent* movement, AnimationGroupsComponent* animationGroups)
		{
			// calculate state
			CharacterState state = stateMachine->getCharacterSM().getNextState(characterState->getBlackboard(), characterState->getState());
			characterState->setState(state);

			// allow movement
			if (!CanMove(state))
			{
				movement->setMoveDirection(ZERO_VECTOR);
				movement->setSightDirection(ZERO_VECTOR);
			}
			movement->setSpeed(IsRunning(state) ? movement->getOriginalSpeed() * 2.0f : movement->getOriginalSpeed());

			// update animation blackboard
			auto& animBlackboard = animationGroups->getBlackboardRef();
			animBlackboard.setValue<std::string>("charState", enum_to_string(state));
			float relativeRotation = (movement->getMoveDirection().rotation() - movement->getSightDirection().rotation()).getValue();

			if (relativeRotation < PI * 0.25f && relativeRotation > -PI * 0.25f)
			{
				animBlackboard.setValue<std::string>("moveDir", "f");
			}
			else if (relativeRotation > PI * 0.75f && relativeRotation < -PI * 0.75f)
			{
				animBlackboard.setValue<std::string>("moveDir", "b");
			}
			else if (relativeRotation < 0.0f)
			{
				animBlackboard.setValue<std::string>("moveDir", "l");
			}
			else
			{
				animBlackboard.setValue<std::string>("moveDir", "r");
			}
		});
	}
}
