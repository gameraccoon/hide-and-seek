#include "Man.h"

#include "../src/ActorFactory.h"

// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateMan(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new Man(world, location);
	}

	const std::string MAN_ID = "Man";

	// register specific factory in actor factory
	const bool registered = ActorFactory::registerActor(MAN_ID, CreateMan);
}

Man::Man(World *ownerWorld, Vector2D location) : DummyMan(ownerWorld, location),
												navigator(ownerWorld),
												destinationPoint(location)
{
	this->type = AT_Living;

	this->speed = 50.0f;

	this->classID = MAN_ID;
}

Man::~Man(void)
{
}

void Man::update(float deltatime)
{
	if (this->destinationPoint == this->location)
	{
		this->destinationPoint = this->navigator.getNextPoint();
		this->direction = (this->destinationPoint - this->location).rotation();
	}

	float stepSize = this->speed * deltatime;
	if (stepSize > (this->destinationPoint - this->location).size())
	{
		this->location = this->destinationPoint;
		this->updateCollision();
		return;
	}
	Vector2D newLocation = this->location + (this->destinationPoint - this->location).ort() * stepSize;
	bool bFree = true;

	// for each actors in the world
	for (std::set<IActor*>::iterator i = this->ownerWorld->allActors.begin(), iEnd = this->ownerWorld->allActors.end(); i != iEnd; i++)
	{
		// if the actor is not this man // test: and it is a static actor
		if ((*i) != this && ((*i)->getType() != AT_Light && (*i)->getType() != AT_Special && (*i)->getType() != AT_Bullet))
		{
			// get an actor's AABB (axis-aligned bounding box)
			BoundingBox box = (*i)->getBoundingBox();
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
		this->location = newLocation;
	}
	
	this->updateCollision();

	// use superclass method
	DummyMan::update(deltatime);
}

void Man::takeDamage(float damageValue,Vector2D impulse)
{
	for (std::set<IActor*>::iterator i = this->ownerWorld->allActors.begin(), iEnd = this->ownerWorld->allActors.end(); i != iEnd; i++)
	{
		if ((*i)->getType() == AT_Living && (*i) != this)
		{
			this->navigator.createNewPath(this->location, (*i)->getLocation());
		}
	}
	
	this->destinationPoint = this->navigator.getNextPoint();
	this->direction = (this->destinationPoint - this->location).rotation();
}