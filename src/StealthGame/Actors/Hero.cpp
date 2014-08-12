#include "Hero.h"

#include <Engine/AI/PlayerRole.h>

#include <Engine/Modules/Collide.h>

Hero::Hero(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Body(world, location),
	step(ZERO_VECTOR)
{
	this->speed = 1.f;

	this->updateActorId("Hero");

	this->setFraction(Fraction::GoodGuys);
	
	if (this->role != nullptr)
		delete this->role;

	this->role = new PlayerRole(world, this);
}

Hero::~Hero(void)
{
}

void Hero::move(Vector2D step)
{
	this->step += step;
}

void Hero::update(float deltatime)
{
	Vector2D step =  this->step * deltatime;

	// if actor's path is free
	if (!Collide::isWillCollide(this, this->getOwnerWorld(), step))
	{
		// accept new position of the man
		this->setLocation(this->getLocation() + step);
	}

	this->step = ZERO_VECTOR;

	// use superclass method
	Body::update(deltatime);
}
