#include "Role.h"

#include "../Lua/LuaAiState.h"

Role::Role(World* world, IBody *body)
{
	this->states.push(new LuaAiState(world, body, this));
	this->body = body;
}

Role::~Role(void)
{
	IAiState *deletedState = this->states.top();
	delete deletedState;
	this->states.pop();
}

void Role::update(float deltaTime)
{

}

void Role::onTakeDamage(IActor* instigator, float damageValue, Vector2D impulse)
{
	this->states.top()->onTakeDamage(instigator, damageValue, impulse);
}

void Role::onSeeEnemy(IActor *enemy)
{
}

void Role::onHearNoise(SoundVolume *sound)
{
}

IBody* Role::getBody()
{
	return this->body;
}