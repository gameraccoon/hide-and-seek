#include "Role.h"

#include "AiState.h"

Role::Role(World* world, IBody *body)
{
	this->states.push(new AiState(world, body, this));
	this->body = body;
}

Role::~Role(void)
{
	this->states.pop();
}

void Role::update(float deltaTime)
{
	//this->states.process(deltaTime);
}

void Role::onTakeDamage(IActor* instigator, float damageValue)
{
	this->states.onTakeDamage(instigator, damageValue);
}

IBody* Role::getBody()
{
	return this->body;
}