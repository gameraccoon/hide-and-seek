#include "StatesStack.h"

#include <cstdlib>

#include "../../Helpers/DebugMethods.h"

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
 	#define RELEASE
  	#define WARN(message)
  	#define WARN_IF(condition, message)
#endif

/**
 * State iterator for StatesStack
 *
 * Used to hide implementation details
 */
class StateIterator
{
public:
	/**
     * @param prewState state iterator which was last
     * @param state state which we pushed into stack
	 */
	StateIterator(StateIterator *prewState, State *state)
	{
		this->prewState = prewState;
		this->currentState = state;
	}

	~StateIterator()
	{
		WARN_IF(!this->currentState, "Trying to destruct StateIterator which has no State");
		delete this->currentState;
	}

	/**
     * Process state code
	 */
	void process()
	{
		WARN_IF(!this->currentState, "Trying to process State which not exist in StateIterator");
		this->currentState->process();
	}

	/**
     * @return iterator of a previous state
	 */
	StateIterator* ReturnLast()
	{
		return this->prewState;
	}

private:
	/**
     * State that stored in this iterator
	 */
	State *currentState;

	/**
     * Previous state
	 *
     * It will become the current when this state
     * will replased from stack
	 */
	StateIterator *prewState;
};

StatesStack::StatesStack()
{
	this->head = NULL;
}

void StatesStack::push(State *newState)
{
	this->head = new StateIterator(this->head, newState);
}

void StatesStack::pop()
{
    // save pointer to state which we will delete
	StateIterator* oldHead = this->head;
	WARN_IF(!this->head, "Trying to 'Pop' from empty StatesStack. It is a fatal error.");
    // set new head
	this->head = this->head->ReturnLast();      
	delete oldHead;
}

void StatesStack::process()
{
	if (this->head != NULL)
	{
		this->head->process();
	}
}
