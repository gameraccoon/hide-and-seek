#include "Role.h"

#include "AiState.h"

Role::Role(World* world, IActor *body)
{
	this->states.push(new AiState(world, body, this));
	this->body = body;
}

Role::~Role(void)
{

}

void Role::update(float deltaTime)
{
	this->states.process(deltaTime);
}

IActor* Role::getBody()
{
	return this->body;
}