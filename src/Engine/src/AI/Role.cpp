#include "AI/Role.h"

Role::Role(World* world, IBody *body)
    : mBody(body)
    , mWorld(world)
{
}

Role::~Role()
{
}

IBody* Role::getBody() const
{
	return mBody;
}

World* Role::getWorld() const
{
	return mWorld;
}
