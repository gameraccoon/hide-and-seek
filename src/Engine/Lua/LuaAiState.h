#ifndef LUA_AI_STATE
#define LUA_AI_STATE

#include "../Core/World.h"
#include "../AI/AiState.h"
#include <LuaInstance.h>

class LuaAiState : public AiState
{
public:
	LuaAiState(World *world, IBody *body, Role *role);
	virtual ~LuaAiState(void);

	/**
	 * Event that executed when body takes damage
	 */
	virtual void onTakeDamage(IActor *instigator, float damageValue, Vector2D impulse) override;
	virtual void onSeeEnemy(IActor *enemy) override;
	virtual void onHearNoise(SoundVolume *sound) override;
private:
	LuaInstance *script;

	float scriptUpdateInterval;
};

#endif