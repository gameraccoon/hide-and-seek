#ifndef AI_STATES_STACK_H
#define AI_STATES_STACK_H

#include "IAiState.h"

/**
 * State iterator in a states stack
 *
 * Helps hidding inner realizations of the AiStatesStack class
 */
class AiStateIterator;

/**
 * Stack of states
 */
class AiStatesStack
{
public:
	AiStatesStack();
	~AiStatesStack();

	/**
     * Set new state
	 *
     * @param newState is a new state which becomes
     * the current state
	 */
	void push(IAiState *newState);
	
	/**
     * Delete the current state
	 *
     * Next state of stack becomes the current state
	 */
	void pop();

	/**
     * Process the code of the current state
	 */
	void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse);
	void onSeeEnemy(IActor *enemy);
	void onHearNoise(SoundVolume *sound);

private:
    /** Stack head */
	AiStateIterator *head;
};

#endif
