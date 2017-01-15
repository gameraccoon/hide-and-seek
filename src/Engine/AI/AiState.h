#pragma once

#include "Role.h"
#include "../Core/World.h"
#include "../AI/IAiState.h"

class AiState : public IAiState
{
public:
	AiState(World *world, IBody *body, Role *role);
	virtual ~AiState();

	/**
	 * Event that executed when body takes damage
	 */
	virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse) override = 0;
	virtual void onSeeEnemy(IActor *enemy) override = 0;
	virtual void onHearNoise(SoundVolume *sound) override = 0;

	void gotoState(std::string stateName);
	void endState();

	virtual bool isEnded() const override;
	virtual void setEnded(bool ended) override;
protected:

	IBody *mBody;
	Role *mRole;
	World *mWorld;

	float mLastExecutionTime;

	bool mEnded;
};
