#include "stdafx.h"
#include "CppUnitTest.h"

#include "../src/States.cpp"

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

			void Process()
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
			void Process()
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
			void Process()
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
			Stack.Push(new TestStateA());
			Stack.Process();
			Assert::AreEqual(TestValueForTestState, 1);
		}

		void testStatesStackOneState()
		{
			
		}

		TEST_METHOD(TestStatesStackMultiplyState)
		{
			// Test processing statestack with one state
			TestValueForTestState = 0;
			StatesStack Stack;
			Stack.Push(new TestStateA());	// A <-
			Stack.Push(new TestStateB());	// A B <-
			Stack.Process();				// Run B->Process()
			Assert::AreEqual(TestValueForTestState, 2);
			TestValueForTestState = 0;
			Stack.Push(new TestStateC());	// A B C <-
			Stack.Process();				// Run C->Process()
			Assert::AreEqual(TestValueForTestState, 3);
			TestValueForTestState = 0;
			Stack.Pop();					// A B <-
			Stack.Process();				// Run B->Process()
			Assert::AreEqual(TestValueForTestState, 2);
			TestValueForTestState = 0;
			Stack.Pop();					// A <-
			Stack.Process();				// Run A->Process()
			Assert::AreEqual(TestValueForTestState, 1);
			TestValueForTestState = 0;
			Stack.Pop();					// Nothing in stack
		}

		};
}