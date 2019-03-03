#include "Actors/Hero.h"

#include <AI/PlayerRole.h>

#include <Modules/Collide.h>
#include <Components/MovementComponent.h>

Hero::Hero(World *world, Vector2D location)
	: Body(world, location)
	, mStep(ZERO_VECTOR)
{
	mSpeed = 1.f;

	updateActorId("Hero");

	setFraction(Fraction::GoodGuys);
	
	mRole = std::make_unique<PlayerRole>(world, this);
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
		auto movementComponent = getSingleComponent<MovementComponent>();
		if (movementComponent != nullptr)
		{
			// accept new position of the man
			movementComponent->setLocation(movementComponent->getLocation() + step);
		}
	}

	step = ZERO_VECTOR;

	// use superclass method
	Body::update(deltatime);
}
