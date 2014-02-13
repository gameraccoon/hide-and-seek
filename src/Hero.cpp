#include "Hero.h"

#include "../src/ActorFactory.h"

Hero::Hero(World *ownerWorld, Vector2D location) : DummyMan(ownerWorld, location),
	step(ZERO_VECTOR)
{
	this->speed = 1.f;

	this->classID = "Hero";
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
	Vector2D newLocation = this->location + this->step * deltatime;
	bool bFree = true;

	// for each actors in the world
	for (std::set<IActor*>::iterator it = this->ownerWorld->allActors.begin(); it != this->ownerWorld->allActors.end(); it++)
	{
		// if the actor is not this man // test: and it is a static actor
		if ((*it) != this && ((*it)->getType() != AT_Light && (*it)->getType() != AT_Special && (*it)->getType() != AT_Bullet))
		{
			// get an actor's AABB (axis-aligned bounding box)
			BoundingBox box = (*it)->getBoundingBox();
			// if the actor's AABB intersects with the Man's AABB (in new Man location)
			if ((box.minX < newLocation.x + this->size.x/2 && newLocation.x - this->size.x/2 < box.maxX)
				&&
				(box.minY < newLocation.y + this->size.y/2 && newLocation.y - this->size.y/2 < box.maxY))
			{
				// actor's path is not free
				bFree = false;
			}
		}
		
		// use superclass method
		DummyMan::update(deltatime);
	}

	// if actor's path is free
	if (bFree)
	{
		// accept new position of the man
		this->location = newLocation;
	}
	
	this->updateCollision();
	this->step = ZERO_VECTOR;
}
