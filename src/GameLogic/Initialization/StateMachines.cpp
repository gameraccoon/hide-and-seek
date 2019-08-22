#include "GameLogic/Initialization/StateMachines.h"

#include "GameData/Enums/MoveDirection4.generated.h"

namespace StateMachines
{
	static void RegisterCharacterSM(FSM::StateMachine<CharacterState, CharacterStateBlackboardKeys>& sm)
	{
		using FSMType = FSM::StateMachine<CharacterState, CharacterStateBlackboardKeys>;
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Walk, CharacterStateBlackboardKeys::TryingToMove, true);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Shoot, CharacterStateBlackboardKeys::TryingToShoot, true);
			sm.addState(CharacterState::Idle, std::move(rules));
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Idle, CharacterStateBlackboardKeys::TryingToMove, false);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::WalkAndShoot, CharacterStateBlackboardKeys::TryingToShoot, true);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Run, CharacterStateBlackboardKeys::ReadyToRun, true);
			sm.addState(CharacterState::Walk, std::move(rules));
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::WalkAndShoot, CharacterStateBlackboardKeys::TryingToMove, true);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Idle, CharacterStateBlackboardKeys::TryingToShoot, false);
			sm.addState(CharacterState::Shoot, std::move(rules));
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Shoot, CharacterStateBlackboardKeys::TryingToMove, false);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Walk, CharacterStateBlackboardKeys::TryingToShoot, false);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Run, CharacterStateBlackboardKeys::ReadyToRun, true);
			sm.addState(CharacterState::WalkAndShoot, std::move(rules));
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Walk, CharacterStateBlackboardKeys::ReadyToRun, false);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterState::Idle, CharacterStateBlackboardKeys::TryingToMove, false);
			sm.addState(CharacterState::Run, std::move(rules));
		}
	}

	static void RegisterCharacterAnimationSM(FSM::StateMachine<CharacterAnimationState, CharacterStateBlackboardKeys>& sm)
	{
		using FSMType = FSM::StateMachine<CharacterAnimationState, CharacterStateBlackboardKeys>;

		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterAnimationState::Move, CharacterStateBlackboardKeys::TryingToMove, true);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterAnimationState::Shoot, CharacterStateBlackboardKeys::TryingToShoot, true);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterAnimationState::Reload, CharacterStateBlackboardKeys::Reloading, true);
			sm.addState(CharacterAnimationState::Idle, std::move(rules));
		}

		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterAnimationState::Idle, CharacterStateBlackboardKeys::TryingToMove, false);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterAnimationState::Reload, CharacterStateBlackboardKeys::Reloading, true);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterAnimationState::Shoot, CharacterStateBlackboardKeys::TryingToShoot, true);
			sm.addState(CharacterAnimationState::Move, std::move(rules));
		}

		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterAnimationState::Idle, CharacterStateBlackboardKeys::Reloading, false);
			sm.addState(CharacterAnimationState::Reload, std::move(rules));
		}

		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterAnimationState::Idle, CharacterStateBlackboardKeys::TryingToShoot, false);
			sm.addState(CharacterAnimationState::Shoot, std::move(rules));
		}
	}

	static void RegisterCharacterLegsAnimationSM(FSM::StateMachine<CharacterLegsAnimationState, CharacterStateBlackboardKeys>& sm)
	{
		using FSMType = FSM::StateMachine<CharacterLegsAnimationState, CharacterStateBlackboardKeys>;

		// Top level states
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterLegsAnimationState::Idle, CharacterStateBlackboardKeys::TryingToMove, false);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterLegsAnimationState::Run, CharacterStateBlackboardKeys::ReadyToRun, true);
			sm.addState(CharacterLegsAnimationState::MetaWalk, std::move(rules));
		}

		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterLegsAnimationState::MetaWalk, CharacterStateBlackboardKeys::TryingToMove, true);
			sm.addState(CharacterLegsAnimationState::Idle, std::move(rules));
		}

		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterLegsAnimationState::MetaWalk, CharacterStateBlackboardKeys::ReadyToRun, false);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(CharacterLegsAnimationState::Idle, CharacterStateBlackboardKeys::TryingToMove, false);
			sm.addState(CharacterLegsAnimationState::Run, std::move(rules));
		}

		// Substates of MetaWalk
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>(CharacterLegsAnimationState::StrafeLeft, CharacterStateBlackboardKeys::TryingToMove, MoveDirection4::Left);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>(CharacterLegsAnimationState::StrafeRight, CharacterStateBlackboardKeys::TryingToMove, MoveDirection4::Right);
			sm.addState(CharacterLegsAnimationState::Walk, std::move(rules));
			sm.linkStates(CharacterLegsAnimationState::Walk, CharacterLegsAnimationState::MetaWalk, true);
		}

		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>(CharacterLegsAnimationState::Walk, CharacterStateBlackboardKeys::TryingToMove, MoveDirection4::Front);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>(CharacterLegsAnimationState::Walk, CharacterStateBlackboardKeys::TryingToMove, MoveDirection4::Back);
			sm.addState(CharacterLegsAnimationState::StrafeLeft, std::move(rules));
			sm.linkStates(CharacterLegsAnimationState::StrafeLeft, CharacterLegsAnimationState::MetaWalk, true);
		}

		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>(CharacterLegsAnimationState::Walk, CharacterStateBlackboardKeys::TryingToMove, MoveDirection4::Front);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>(CharacterLegsAnimationState::Walk, CharacterStateBlackboardKeys::TryingToMove, MoveDirection4::Back);
			sm.addState(CharacterLegsAnimationState::StrafeRight, std::move(rules));
			sm.linkStates(CharacterLegsAnimationState::StrafeRight, CharacterLegsAnimationState::MetaWalk, true);
		}
	}

	void RegisterStateMachines(StateMachineComponent *stateMachine)
	{
		if (!stateMachine)
		{
			return;
		}

		RegisterCharacterSM(stateMachine->getCharacterSMRef());
		RegisterCharacterAnimationSM(stateMachine->getCharacterAnimationSMRef());
		RegisterCharacterLegsAnimationSM(stateMachine->getCharacterLegsAnimationSMRef());
	}
}
