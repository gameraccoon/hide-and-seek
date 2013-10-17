#include <cfixcc.h>

#include "..\src\States.h"

int TestValueForTestState;
class testStates : public cfixcc::TestFixture
{
private:
	

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
	
	

	void testStatesStackOneState()
	{
		// Test processing statestack with one state
		TestValueForTestState = 0;
		StatesStack Stack;
		Stack.Push(new TestStateA());
		Stack.Process();
		CFIXCC_ASSERT_EQUALS(TestValueForTestState, 1);
	}

	void testStatesStackMultiplyState()
	{
		// Test processing statestack with one state
		TestValueForTestState = 0;
		StatesStack Stack;
		Stack.Push(new TestStateA());	// A <-
		Stack.Push(new TestStateB());	// A B <-
		Stack.Process();				// Run B->Process()
		CFIXCC_ASSERT_EQUALS(TestValueForTestState, 2);
		TestValueForTestState = 0;
		Stack.Push(new TestStateC());	// A B C <-
		Stack.Process();				// Run C->Process()
		CFIXCC_ASSERT_EQUALS(TestValueForTestState, 3);
		TestValueForTestState = 0;
		Stack.Pop();					// A B <-
		Stack.Process();				// Run B->Process()
		CFIXCC_ASSERT_EQUALS(TestValueForTestState, 2);
		TestValueForTestState = 0;
		Stack.Pop();					// A <-
		Stack.Process();				// Run A->Process()
		CFIXCC_ASSERT_EQUALS(TestValueForTestState, 1);
		TestValueForTestState = 0;
		Stack.Pop();					// Nothing in stack
	}

};

CFIXCC_BEGIN_CLASS(testStates)
	CFIXCC_METHOD(testStatesStackOneState)
	CFIXCC_METHOD(testStatesStackMultiplyState)
CFIXCC_END_CLASS()

