#include "Hero.h"

#include "../../Engine/AI/PlayerRole.h"

Hero::Hero(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Body(world, location),
	step(ZERO_VECTOR)
{
	this->speed = 1.f;

	this->updateActorId("Hero");
	
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
	Vector2D newLocation = this->getLocation() + this->step * deltatime;
	bool bFree = true;

	// for each actors in the world
	for (auto const &actor : this->getOwnerWorld()->allActors)
	{
		// if the actor is not this man // test: and it is a static actor
		if (actor != this && (actor->getType() != ActorType::Light && actor->getType() != ActorType::Special && actor->getType() != ActorType::Bullet))
		{
			// get an actor's AABB (axis-aligned bounding box)
			BoundingBox box = actor->getBoundingBox();
			// if the actor's AABB intersects with the Man's AABB (in new Man location)
			if ((box.minX < newLocation.x + this->size.x/2 && newLocation.x - this->size.x/2 < box.maxX)
				&&
				(box.minY < newLocation.y + this->size.y/2 && newLocation.y - this->size.y/2 < box.maxY))
			{
				// actor's path is not free
				bFree = false;
			}
		}
	}

	// if actor's path is free
	if (bFree)
	{
		// accept new position of the man
		this->setLocation(newLocation);
	}
	
	this->updateCollision();
	this->step = ZERO_VECTOR;

	// use superclass method
	Body::update(deltatime);
}
