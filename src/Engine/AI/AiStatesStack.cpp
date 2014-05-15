#include "AiStatesStack.h"

#include <cstdlib>

#include "../Helpers/DebugMethods.h"

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
class AiStateIterator
{
public:
	/**
     * @param prewState state iterator which was last
     * @param state state which we pushed into stack
	 */
	AiStateIterator(AiStateIterator *prewState, IAiState *state)
	{
		this->prewState = prewState;
		this->currentState = state;
	}

	~AiStateIterator()
	{
		if (this->currentState != nullptr)
		{
			delete this->currentState;
			this->currentState = nullptr;
		}
	}

	/**
     * Process state code
	 */
	void onTakeDamage(IActor *instigator, float damageValue)
	{
		this->currentState->onTakeDamage(instigator, damageValue);
	}

	/**
     * @return iterator of a previous state
	 */
	AiStateIterator* ReturnLast()
	{
		return this->prewState;
	}

private:
	/**
     * State that stored in this iterator
	 */
	IAiState *currentState;

	/**
     * Previous state
	 *
     * It will become the current when this state
     * will replased from stack
	 */
	AiStateIterator *prewState;
};

AiStatesStack::AiStatesStack()
{
	this->head = nullptr;
}

AiStatesStack::~AiStatesStack()
{
	while (this->head != nullptr)
	{
		this->pop();
	}
}

void AiStatesStack::push(IAiState *newState)
{
	this->head = new AiStateIterator(this->head, newState);
}

void AiStatesStack::pop()
{
    // save pointer to state which we will delete
	AiStateIterator* oldHead = this->head;
    // set new head
	this->head = this->head->ReturnLast();      
	delete oldHead;
}

void AiStatesStack::onTakeDamage(IActor *instigator, float damageValue)
{
	if (this->head != nullptr)
	{
		this->head->onTakeDamage(instigator, damageValue);
	}
}
