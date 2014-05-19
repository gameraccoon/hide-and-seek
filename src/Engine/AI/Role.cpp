#include "Role.h"

#include "../Lua/LuaAiState.h"

Role::Role(World* world, IBody *body)
{
	this->body = body;
}

Role::~Role(void)
{
}

IBody* Role::getBody()
{
	return this->body;
}