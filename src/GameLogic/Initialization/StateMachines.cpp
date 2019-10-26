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

	static void RegisterCharacterAnimationSM(FSM::StateMachine<std::string, std::string>& sm)
	{
		using FSMType = FSM::StateMachine<std::string, std::string>;

		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, std::string>("move", "charState", enum_to_string(CharacterState::Walk));
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, std::string>("move", "charState", enum_to_string(CharacterState::Run));
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, std::string>("shoot", "charState", enum_to_string(CharacterState::Shoot));
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, std::string>("shoot", "charState", enum_to_string(CharacterState::WalkAndShoot));
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, std::string>("reload", "charState", enum_to_string(CharacterState::Reload));
			sm.addState("idle", std::move(rules));
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, std::string>("idle", "charState", enum_to_string(CharacterState::Idle));
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, std::string>("reload", "charState", enum_to_string(CharacterState::Reload));
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, std::string>("shoot", "charState", enum_to_string(CharacterState::Shoot));
			sm.addState("move", std::move(rules));
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>("idle", enum_to_string(CharacterStateBlackboardKeys::Reloading), false);
			sm.addState("reload", std::move(rules));
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>("idle", enum_to_string(CharacterStateBlackboardKeys::TryingToShoot), false);
			sm.addState("shoot", std::move(rules));
		}
	}

	static void RegisterCharacterLegsAnimationSM(FSM::StateMachine<std::string, std::string>& sm)
	{
		using FSMType = FSM::StateMachine<std::string, std::string>;

		// Top level states
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>("idle", enum_to_string(CharacterStateBlackboardKeys::TryingToMove), false);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>("run", enum_to_string(CharacterStateBlackboardKeys::ReadyToRun), true);
			sm.addState("metawalk", std::move(rules));
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>("metawalk", enum_to_string(CharacterStateBlackboardKeys::TryingToMove), true);
			sm.addState("idle", std::move(rules));
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>("metawalk", enum_to_string(CharacterStateBlackboardKeys::ReadyToRun), false);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>("idle", enum_to_string(CharacterStateBlackboardKeys::TryingToMove), false);
			sm.addState("run", std::move(rules));
		}
		// Substates of MetaWalk
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>("strafeleft", "moveDir", MoveDirection4::Left);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>("straferight", "moveDir", MoveDirection4::Right);
			sm.addState("walk", std::move(rules));
			sm.linkStates("walk", "metawalk", true);
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>("walk", "moveDir", MoveDirection4::Front);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>("walk", "moveDir", MoveDirection4::Back);
			sm.addState("strafeleft", std::move(rules));
			sm.linkStates("strafeleft", "metawalk", false);
		}
		{
			FSMType::StateLinkRules rules;
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>("walk", "moveDir", MoveDirection4::Front);
			rules.emplaceLink<FSM::LinkRules::VariableEqualLink, MoveDirection4>("walk", "moveDir", MoveDirection4::Back);
			sm.addState("straferight", std::move(rules));
			sm.linkStates("straferight", "metawalk", false);
		}
	}

	void RegisterStateMachines(StateMachineComponent *stateMachine)
	{
		if (!stateMachine)
		{
			return;
		}

		RegisterCharacterSM(stateMachine->getCharacterSMRef());
		RegisterCharacterAnimationSM(stateMachine->getAnimationSMsRef()["handgun"]);
		RegisterCharacterLegsAnimationSM(stateMachine->getAnimationSMsRef()["feet"]);
	}
}
