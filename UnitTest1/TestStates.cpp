#include "stdafx.h"
#include "CppUnitTest.h"

#include "../src/Engine/Modules/States/State.cpp"
#include "../src/Engine/Modules/States/StatesStack.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestStates
{	
	int TestValueForTestState;
	TEST_CLASS(TestStates)
	{
		class TestStateA:public State
		{
		public:
			~TestStateA()
			{
			}

			virtual void process(float deltatime) override final
			{
				TestValueForTestState = 1;
			}
		};

		class TestStateB:public State
		{
		public:
			~TestStateB()
			{
			}

			virtual void process(float deltatime) override final
			{
				TestValueForTestState = 2;
			}
		};

		class TestStateC:public State
		{
		public:
			~TestStateC()
			{
			}

			virtual void process(float deltatime) override final
			{
				TestValueForTestState = 3;
			}
		};
	public:
		TEST_METHOD(TestStatesStackOneState)
		{
			// Test processing statestack with one state
			TestValueForTestState = 0;
			StatesStack Stack;
			Stack.push(new TestStateA());
			Stack.process(1.f);
			Assert::AreEqual(1, TestValueForTestState);
		}

		TEST_METHOD(TestStatesStackMultiplyState)
		{
			// Test processing statestack with one state
			TestValueForTestState = 0;
			StatesStack Stack;
			Stack.push(new TestStateA());	// A <-
			Stack.push(new TestStateB());	// A B <-
			Stack.process(1.f);				// Run B->Process()
			Assert::AreEqual(2, TestValueForTestState);
			TestValueForTestState = 0;
			Stack.push(new TestStateC());	// A B C <-
			Stack.process(1.f);				// Run C->Process()
			Assert::AreEqual(3, TestValueForTestState);
			TestValueForTestState = 0;
			Stack.pop();					// A B <-
			Stack.process(1.f);				// Run B->Process()
			Assert::AreEqual(2, TestValueForTestState);
			TestValueForTestState = 0;
			Stack.pop();					// A <-
			Stack.process(1.f);				// Run A->Process()
			Assert::AreEqual(1, TestValueForTestState);
			TestValueForTestState = 0;
			Stack.pop();					// Nothing in stack
		}
	};
}