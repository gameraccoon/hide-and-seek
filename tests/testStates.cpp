#include <cfixcc.h>

#include "..\src\States.h"

int TestValueForTestState;
class TestStates : public cfixcc::TestFixture
{
private:
	
	class TestStateA:public State
	{
	public:
		~TestStateA()
		{
		}

		void process()
		{
			::TestValueForTestState = 1;
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
			::TestValueForTestState = 2;
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
			::TestValueForTestState = 3;
		}
	};
public:

	void testStatesStackOneState()
	{
		// Test processing statestack with one state
		::TestValueForTestState = 0;
		StatesStack stack;
		stack.push(new TestStateA());
		stack.process();
		CFIXCC_ASSERT_EQUALS(::TestValueForTestState, 1);
	}

	void testStatesStackMultiplyState()
	{
		// Test processing statestack with one state
		::TestValueForTestState = 0;
		StatesStack stack;
		stack.push(new TestStateA());	// A <-
		stack.push(new TestStateB());	// A B <-
		stack.process();				// Run B->Process()
		CFIXCC_ASSERT_EQUALS(::TestValueForTestState, 2);
		::TestValueForTestState = 0;
		stack.push(new TestStateC());	// A B C <-
		stack.process();				// Run C->Process()
		CFIXCC_ASSERT_EQUALS(::TestValueForTestState, 3);
		::TestValueForTestState = 0;
		stack.pop();					// A B <-
		stack.process();				// Run B->Process()
		CFIXCC_ASSERT_EQUALS(::TestValueForTestState, 2);
		::TestValueForTestState = 0;
		stack.pop();					// A <-
		stack.process();				// Run A->Process()
		CFIXCC_ASSERT_EQUALS(::TestValueForTestState, 1);
		::TestValueForTestState = 0;
		stack.pop();					// Nothing in stack
	}
};

CFIXCC_BEGIN_CLASS(TestStates)
	CFIXCC_METHOD(testStatesStackOneState)
	CFIXCC_METHOD(testStatesStackMultiplyState)
CFIXCC_END_CLASS()
