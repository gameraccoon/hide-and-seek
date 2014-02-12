#include "Hero.h"

#include "../src/ActorFactory.h"

Hero::Hero(World *ownerWorld, Vector2D location) : DummyMan(ownerWorld, location),
												Step(ZeroVector)
{
	Speed = 1.f;

	ClassID = "Hero";
}

Hero::~Hero(void)
{
}

void Hero::Move(Vector2D step)
{
	Step += step;
}

void Hero::Update(float deltaTime)
{
	Vector2D newLocation = Location + Step * deltaTime;
	bool bFree = true;

	// for each actors in the world
	for (std::set<IActor*>::iterator it = OwnerWorld->AllActors.begin(); it != OwnerWorld->AllActors.end(); it++)
	{
		// if the actor is not this man // test: and it is a static actor
		if ((*it) != this && ((*it)->GetType() != AT_Light && (*it)->GetType() != AT_Special && (*it)->GetType() != AT_Bullet))
		{
			// get an actor's AABB (axis-aligned bounding box)
			BoundingBox box = (*it)->GetBoundingBox();
			// if the actor's AABB intersects with the Man's AABB (in new Man location)
			if ((box.MinX < newLocation.X + Size.X/2 && newLocation.X - Size.X/2 < box.MaxX)
				&&
				(box.MinY < newLocation.Y + Size.Y/2 && newLocation.Y - Size.Y/2 < box.MaxY))
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
		Location = newLocation;
	}
	
	UpdateCollision();
	Step = ZeroVector;
}