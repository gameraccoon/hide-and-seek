#ifndef I_AI_STATE
#define I_AI_STATE

#include "../Core/IActor.h"
#include "../Actors/SoundVolume.h"

class IAiState
{
public:
	/**
	 * Event that executed when body takes damage
	 */
	virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse) = 0;
	virtual void onSeeEnemy(IActor *enemy) = 0;
	virtual void onHearNoise(SoundVolume *sound) = 0;
};

#endif