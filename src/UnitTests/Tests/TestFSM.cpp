#include <gtest/gtest.h>

#include "GameData/FSM/StateMachine.h"

enum class TestStates
{
	StateOne,
	StateTwo,
	StateThree,
	StateFour
};

TEST(FSM, BasicStateAndLink)
{
	using TestFSM = FSM::StateMachine<TestStates>;

	TestFSM fsm;

	TestFSM::StateLinks stateOneLinks;
	stateOneLinks.links.emplace_back(TestStates::StateTwo, std::make_unique<FSM::LinkRules::FunctorLink>([](const FSM::Blackboard&){
		return true;
	}));
	fsm.addState(TestStates::StateOne, std::move(stateOneLinks));

	fsm.addState(TestStates::StateTwo, TestFSM::StateLinks());

	fsm.setState(TestStates::StateOne);
	fsm.update();

	EXPECT_EQ(TestStates::StateTwo, fsm.getCurrentState());
}

TEST(FSM, EqualityLinkRule)
{
	using TestFSM = FSM::StateMachine<TestStates>;

	TestFSM fsm;

	TestFSM::StateLinks stateOneLinks;
	stateOneLinks.links.emplace_back(TestStates::StateTwo, std::make_unique<FSM::LinkRules::VariableEqualLink<bool>>("test", true));
	fsm.addState(TestStates::StateOne, std::move(stateOneLinks));

	TestFSM::StateLinks stateTwoLinks;
	stateTwoLinks.links.emplace_back(TestStates::StateOne, std::make_unique<FSM::LinkRules::VariableEqualLink<bool>>("test", false));
	fsm.addState(TestStates::StateTwo, std::move(stateTwoLinks));

	fsm.setState(TestStates::StateOne);
	fsm.update();

	EXPECT_EQ(TestStates::StateOne, fsm.getCurrentState());

	fsm.getBlackboardRef().setValue("test", true);
	fsm.update();

	EXPECT_EQ(TestStates::StateTwo, fsm.getCurrentState());

	fsm.getBlackboardRef().setValue("test", false);
	fsm.update();

	EXPECT_EQ(TestStates::StateOne, fsm.getCurrentState());
}

TEST(FSM, DoubleLinkJump)
{
	using TestFSM = FSM::StateMachine<TestStates>;

	TestFSM fsm;

	TestFSM::StateLinks stateOneLinks;
	stateOneLinks.links.emplace_back(TestStates::StateTwo, std::make_unique<FSM::LinkRules::VariableEqualLink<bool>>("stepOne", true));
	fsm.addState(TestStates::StateOne, std::move(stateOneLinks));

	TestFSM::StateLinks stateTwoLinks;
	stateTwoLinks.links.emplace_back(TestStates::StateThree, std::make_unique<FSM::LinkRules::VariableEqualLink<bool>>("stepTwo", true));
	fsm.addState(TestStates::StateTwo, std::move(stateTwoLinks));

	fsm.addState(TestStates::StateTwo, TestFSM::StateLinks());

	fsm.setState(TestStates::StateOne);
	fsm.update();

	EXPECT_EQ(TestStates::StateOne, fsm.getCurrentState());

	fsm.getBlackboardRef().setValue("stepTwo", true);
	fsm.update();

	EXPECT_EQ(TestStates::StateOne, fsm.getCurrentState());

	fsm.getBlackboardRef().setValue("stepOne", true);
	fsm.update();

	EXPECT_EQ(TestStates::StateThree, fsm.getCurrentState());
}

TEST(FSM, Blackboard)
{
	FSM::Blackboard blackboard;

	blackboard.setValue<int>("test", 1);

	EXPECT_EQ(1, blackboard.getValue<int>("test"));
	EXPECT_EQ(0, blackboard.getValue<int>("non-existent"));
	EXPECT_EQ(3, blackboard.getValue<int>("non-existent", 3));
}
