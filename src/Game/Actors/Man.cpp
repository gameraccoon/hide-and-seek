#include "Man.h"

#include "../AI/AiRole.h"

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


Man::Man(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Body(world, location)
{
	this->setType(ActorType::Living);

	this->speed = 50.0f;

	this->updateActorId(CLASS_ID);

	if (this->role != nullptr)
		delete this->role;
	this->role = new AiRole(world, this);
}

Man::~Man(void)
{
}

void Man::update(float deltatime)
{
	if (this->tempLocation == this->getLocation())
	{
		this->findNextPathPoint();
	}

	float stepSize = this->speed * deltatime;

	if (stepSize < (this->tempLocation - this->getLocation()).size())
	{
		Vector2D newLocation = this->getLocation() + (this->tempLocation - this->getLocation()).ort() * stepSize;
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

void Man::hit(IActor *instigator, float damageValue, Vector2D impulse)
{
	Body::hit(instigator, damageValue, impulse);
}