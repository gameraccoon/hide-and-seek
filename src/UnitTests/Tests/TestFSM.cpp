#include <gtest/gtest.h>

#include "GameData/FSM/StateMachine.h"

enum class TestStates
{
	StateOne,
	StateTwo,
	StateThree,
	StateFour
};

enum class BlackboardTestValues
{
	One,
	Two,
	Three
};

TEST(FSM, BasicStateAndLink)
{
	using TestFSM = FSM::StateMachine<TestStates, BlackboardTestValues>;

	TestFSM fsm;

	TestFSM::StateLinkRules stateOneLinks;
	stateOneLinks.emplaceLink<FSM::LinkRules::FunctorLink>(TestStates::StateTwo, [](const TestFSM::BlackboardType&){
		return true;
	});
	fsm.addState(TestStates::StateOne, std::move(stateOneLinks));

	fsm.addState(TestStates::StateTwo, TestFSM::StateLinkRules());

	fsm.setState(TestStates::StateOne);
	fsm.update();

	EXPECT_EQ(TestStates::StateTwo, fsm.getCurrentState());
}

TEST(FSM, EqualityLinkRule)
{
	using TestFSM = FSM::StateMachine<TestStates, BlackboardTestValues>;

	TestFSM fsm;

	TestFSM::StateLinkRules stateOneLinks;
	stateOneLinks.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(TestStates::StateTwo, BlackboardTestValues::One, true);
	fsm.addState(TestStates::StateOne, std::move(stateOneLinks));

	TestFSM::StateLinkRules stateTwoLinks;
	stateTwoLinks.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(TestStates::StateOne, BlackboardTestValues::One, false);
	fsm.addState(TestStates::StateTwo, std::move(stateTwoLinks));

	fsm.setState(TestStates::StateOne);
	fsm.update();

	EXPECT_EQ(TestStates::StateOne, fsm.getCurrentState());

	fsm.getBlackboardRef().setValue<bool>(BlackboardTestValues::One, true);
	fsm.update();

	EXPECT_EQ(TestStates::StateTwo, fsm.getCurrentState());

	fsm.getBlackboardRef().setValue<bool>(BlackboardTestValues::One, false);
	fsm.update();

	EXPECT_EQ(TestStates::StateOne, fsm.getCurrentState());
}

TEST(FSM, DoubleLinkJump)
{
	using TestFSM = FSM::StateMachine<TestStates, std::string>;

	TestFSM fsm;

	TestFSM::StateLinkRules stateOneLinks;
	stateOneLinks.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(TestStates::StateTwo, "stepOne", true);
	fsm.addState(TestStates::StateOne, std::move(stateOneLinks));

	TestFSM::StateLinkRules stateTwoLinks;
	stateTwoLinks.emplaceLink<FSM::LinkRules::VariableEqualLink, bool>(TestStates::StateThree, "stepTwo", true);
	fsm.addState(TestStates::StateTwo, std::move(stateTwoLinks));

	fsm.addState(TestStates::StateTwo, TestFSM::StateLinkRules());

	fsm.setState(TestStates::StateOne);
	fsm.update();

	EXPECT_EQ(TestStates::StateOne, fsm.getCurrentState());

	fsm.getBlackboardRef().setValue<bool>("stepTwo", true);
	fsm.update();

	EXPECT_EQ(TestStates::StateOne, fsm.getCurrentState());

	fsm.getBlackboardRef().setValue<bool>("stepOne", true);
	fsm.update();

	EXPECT_EQ(TestStates::StateThree, fsm.getCurrentState());
}

TEST(FSM, Blackboard)
{
	{
		FSM::Blackboard<std::string> blackboard;

		blackboard.setValue<int>("test", 1);

		EXPECT_EQ(1, blackboard.getValue<int>("test"));
		EXPECT_EQ(0, blackboard.getValue<int>("non-existent"));
		EXPECT_EQ(3, blackboard.getValue<int>("non-existent", 3));
	}

	{
		FSM::Blackboard<BlackboardTestValues> blackboard;

		blackboard.setValue<int>(BlackboardTestValues::One, 1);

		EXPECT_EQ(1, blackboard.getValue<int>(BlackboardTestValues::One));
		EXPECT_EQ(0, blackboard.getValue<int>(BlackboardTestValues::Two));
		EXPECT_EQ(3, blackboard.getValue<int>(BlackboardTestValues::Two, 3));
	}
}
