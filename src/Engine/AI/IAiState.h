#ifndef I_AI_STATE
#define I_AI_STATE

#include "../Core/IActor.h"

class IAiState
{
public:
	/**
	 * Event that executed when body takes damage
	 */
	virtual void onTakeDamage(IActor* instigator, float damageValue) = 0;
};

#endif