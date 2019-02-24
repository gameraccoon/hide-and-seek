#include "Actors/Hero.h"

#include <AI/PlayerRole.h>

#include <Modules/Collide.h>

Hero::Hero(World *world, Vector2D location, Vector2D /*scale*/, Rotator )
	: Body(world, location)
	, mStep(ZERO_VECTOR)
{
	mSpeed = 1.f;

	updateActorId("Hero");

	setFraction(Fraction::GoodGuys);
	
	if (mRole != nullptr)
		delete mRole;

	mRole = new PlayerRole(world, this);
}

void Hero::move(Vector2D step)
{
	mStep += step;
}

void Hero::update(float deltatime)
{
	Vector2D step =  mStep * deltatime;

	// if actor's path is free
	if (!Collide::isWillCollide(this, getOwnerWorld(), step))
	{
		// accept new position of the man
		setLocation(getLocation() + step);
	}

	step = ZERO_VECTOR;

	// use superclass method
	Body::update(deltatime);
}
