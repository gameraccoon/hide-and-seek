#include "Man.h"


// ## This is automatic generated text. Pleace do not change it.
// ## Registration in ActorFactory
#include "../../Engine/Modules/ActorFactory.h"
// unnamed namespase to hide from another places
namespace
{
	// specific factory
	IActor* CreateMan(World *world, const Vector2D location, const Vector2D scale, const Rotator rotation)
	{
		return new Man(world, location, scale, rotation);
	}

	const std::string CLASS_ID = "Man";

	// register specific factory in actor factory
	const bool registered = ActorFactory::Factory().registerActor(CLASS_ID, CreateMan);
}
// ## End of automatic generated text


Man::Man(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Body(world, location)
{
	this->type = AT_Living;

	this->speed = 50.0f;

	this->updateActorId(CLASS_ID);
}

Man::~Man(void)
{
}

void Man::update(float deltatime)
{
	if (this->tempLocation == this->location)
	{
		this->findNextPathPoint();
	}

	float stepSize = this->speed * deltatime;

	if (stepSize < (this->tempLocation - this->location).size())
	{
		Vector2D newLocation = this->location + (this->tempLocation - this->location).ort() * stepSize;
		bool bFree = true;

		// for each actors in the world
		for (auto const &actor : this->ownerWorld->allActors)
		{
			// if the actor is not this man // test: and it is a static actor
			if (actor != this && (actor->getType() != AT_Light && actor->getType() != AT_Special && actor->getType() != AT_Bullet))
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
					break;
				}
			}
		}

		// if actor's path is free
		if (bFree)
		{
			// accept new position of the man
			this->setLocation(newLocation);
		}
		else
		{
			findNextPathPoint();
		}
	}
	else
	{
		this->setLocation(this->tempLocation);
	}

	this->updateCollision();

	// use superclass method
	Body::update(deltatime);
}

void Man::takeDamage(float damageValue, Vector2D impulse)
{
	for (auto const &actor : this->ownerWorld->allActors)
	{
		if (actor->getType() == AT_Living && actor != this)
		{
			//this->follow(foundActor);
			break;
		}
	}

	Body::takeDamage(damageValue, impulse);
}