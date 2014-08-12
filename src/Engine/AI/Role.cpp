#include "Role.h"

Role::Role(World* world, IBody *body)
{
	this->body = body;
	this->world = world;
}

Role::~Role(void)
{
}

IBody* Role::getBody() const
{
	return this->body;
}

World* Role::getWorld() const
{
	return this->world;
}