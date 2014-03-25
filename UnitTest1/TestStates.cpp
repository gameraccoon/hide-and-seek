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
				TestValueForTestState = 0;
			}

			void process()
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

			void process()
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

			void process()
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
			Stack.process();
			Assert::AreEqual(TestValueForTestState, 1);
		}

		TEST_METHOD(TestStatesStackMultiplyState)
		{
			// Test processing statestack with one state
			TestValueForTestState = 0;
			StatesStack Stack;
			Stack.push(new TestStateA());	// A <-
			Stack.push(new TestStateB());	// A B <-
			Stack.process();				// Run B->Process()
			Assert::AreEqual(TestValueForTestState, 2);
			TestValueForTestState = 0;
			Stack.push(new TestStateC());	// A B C <-
			Stack.process();				// Run C->Process()
			Assert::AreEqual(TestValueForTestState, 3);
			TestValueForTestState = 0;
			Stack.pop();					// A B <-
			Stack.process();				// Run B->Process()
			Assert::AreEqual(TestValueForTestState, 2);
			TestValueForTestState = 0;
			Stack.pop();					// A <-
			Stack.process();				// Run A->Process()
			Assert::AreEqual(TestValueForTestState, 1);
			TestValueForTestState = 0;
			Stack.pop();					// Nothing in stack
		}
	};
}