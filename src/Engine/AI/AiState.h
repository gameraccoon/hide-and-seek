#ifndef AI_STATE
#define AI_STATE

#include "Role.h"
#include "../Core/World.h"
#include "../AI/IAiState.h"
#include "../Lua/LuaInstance.h"

class AiState : public IAiState
{
public:
	AiState(World *world, IBody *body, Role *role);
	virtual ~AiState(void);

	/**
	 * Event that executed when body takes damage
	 */
	virtual void onTakeDamage(IActor* instigator, float damageValue) override;
private:
	LuaInstance *script;

	IBody *body;
	Role *role;

	float scriptUpdateInterval;
	float lastExecutionTime;
};

#endif