#include "GameLogic/Game.h"

#include <memory>

#include "HAL/Base/Engine.h"

#include "Utils/World/GameDataLoader.h"

#include "GameLogic/Systems/RenderSystem.h"
#include "GameLogic/Systems/ControlSystem.h"
#include "GameLogic/Systems/CollisionSystem.h"
#include "GameLogic/Systems/ResourceStreamingSystem.h"
#include "GameLogic/Systems/AiSystem.h"
#include "GameLogic/Systems/CharacterStateSystem.h"
#include "GameLogic/Systems/DebugDrawSystem.h"
#include "GameLogic/Systems/MovementSystem.h"

#include "GameLogic/ComponentsRegistration.h"

#include "GameData/Components/StateMachineComponent.generated.h"

// test
static void FillTestStateMachine(StateMachineComponent* stateMachine)
{
	if (!stateMachine)
	{
		return;
	}

	using FSMType = FSM::StateMachine<CharacterState, CharacterStateBlackboardKeys>;
	FSMType& sm = stateMachine->getCharacterSMRef();
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

void Game::start(ArgumentsParser& arguments)
{
	initResources();
	initSystems();

	ComponentsRegistration::RegisterComponents(mComponentFactory);

	mWorldHolder.world = &mWorld;
	mWorldHolder.gameData = &mGameData;

	GameDataLoader::LoadWorld(mWorld, arguments.getArgumentValue("world", "test"), mComponentFactory);
	GameDataLoader::LoadGameData(mGameData, arguments.getArgumentValue("gameData", "gameData"), mComponentFactory);

	// test
	auto [sm] = mGameData.getGameComponents().getComponents<StateMachineComponent>();
	FillTestStateMachine(sm);

	// start the main loop
	getEngine()->start(this);
}

void Game::setKeyState(int key, bool isPressed)
{
	mKeyStates.updateState(key, isPressed);
}

void Game::update(float dt)
{
	mTime.update(dt);
	mSystemsManager.update();
	mKeyStates.clearLastFrameState();
}

void Game::initSystems()
{
	mSystemsManager.registerSystem<ControlSystem>(mWorldHolder, getEngine(), &mKeyStates);
	mSystemsManager.registerSystem<AiSystem>(mWorldHolder);
	mSystemsManager.registerSystem<CharacterStateSystem>(mWorldHolder);
	mSystemsManager.registerSystem<MovementSystem>(mWorldHolder, mTime);
	mSystemsManager.registerSystem<CollisionSystem>(mWorldHolder);
	mSystemsManager.registerSystem<ResourceStreamingSystem>(mWorldHolder, getResourceManager());
	mSystemsManager.registerSystem<RenderSystem>(mWorldHolder, getEngine(), getResourceManager());
	mSystemsManager.registerSystem<DebugDrawSystem>(mWorldHolder, getEngine(), getResourceManager());
}

void Game::initResources()
{
	getResourceManager()->loadAtlasesData("resources/atlas/atlas-list.json");
}
