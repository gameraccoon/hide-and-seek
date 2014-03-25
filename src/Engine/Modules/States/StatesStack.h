#ifndef STATES_H
#define STATES_H

#include "State.h"

/**
 * State iterator in a states stack
 *
 * Helps hidding inner realizations of the StatesStack class
 */
class StateIterator;

/**
 * Stack of states
 */
class StatesStack
{
public:
	StatesStack();
	~StatesStack();

	/**
     * Set new state
	 *
     * @param newState is a new state which becomes
     * the current state
	 */
	void push(State *newState);
	
	/**
     * Delete the current state
	 *
     * Next state of stack becomes the current state
	 */
	void pop();

	/**
     * Process the code of the current state
	 */
	void process(float deltatime);

private:
    /** Stack head */
	StateIterator *head;
};

#endif
