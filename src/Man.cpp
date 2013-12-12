#include "Man.h"

Man::Man(World *ownerWorld, Vector2D location) : DummyMan(ownerWorld, location),
												Navigator(ownerWorld),
												DestinationPoint(location)
{
	Type = AT_Living;

	Speed = 50.0f;
}

Man::~Man(void)
{
}

void Man::Update(float deltaTime)
{
	if (DestinationPoint == Location)
	{
		DestinationPoint = Navigator.GetNextPoint();
		Direction = (DestinationPoint - Location).GetRotation();
	}

	float stepSize = Speed * deltaTime;
	if (stepSize > (DestinationPoint - Location).Size())
	{
		Location = DestinationPoint;
		UpdateCollision();
		return;
	}
	Vector2D newLocation = Location + (DestinationPoint - Location).Ort() * stepSize;
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
}

void Man::TakeDamage(float damageValue,Vector2D impulse)
{
	for (std::set<IActor*>::iterator it = OwnerWorld->AllActors.begin(); it != OwnerWorld->AllActors.end(); it++)
	{
		if ((*it)->GetType() == AT_Living && (*it) != this)
		{
			Navigator.CreateNewPath(Location, (*it)->GetLocation());
		}
	}
	
	DestinationPoint = Navigator.GetNextPoint();
	Direction = (DestinationPoint - Location).GetRotation();
}