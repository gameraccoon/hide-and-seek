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


Man::Man(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Body(world, location),
												navigator(world),
												destinationPoint(location)
{
	this->type = AT_Living;

	this->speed = 50.0f;

	this->target = NULL;

	this->classID = CLASS_ID;
}

Man::~Man(void)
{
}

void Man::update(float deltatime)
{
	if (this->destinationPoint == this->location)
	{
		this->findNextPathPoint();
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
				break;
			}
		}
	}

	// if actor's path is free
	if (bFree)
	{
		// accept new position of the man
		this->location = newLocation;
	}
	else
	{
		findNextPathPoint();
	}
	
	this->updateCollision();

	// use superclass method
	Body::update(deltatime);
}

void Man::takeDamage(float damageValue, Vector2D impulse)
{
	for (std::set<IActor*>::iterator i = this->ownerWorld->allActors.begin(), iEnd = this->ownerWorld->allActors.end(); i != iEnd; i++)
	{
		IActor* foundActor = (*i);
		if (foundActor->getType() == AT_Living && foundActor != this)
		{
			this->target = foundActor;
			findNextPathPoint();
			break;
		}
	}

	Body::takeDamage(damageValue, impulse);
}

void Man::findNextPathPoint()
{
	if (this->target != NULL)
	{
		RayTrace ray(this->ownerWorld, this->getLocation(), this->target->getLocation());
		IActor* tracedActor = ray.trace();

		if (tracedActor == this->target || tracedActor == NULL)
		{
			this->destinationPoint = target->getLocation();
		}
		else
		{
			this->navigator.createNewPath(this->getLocation(), this->target->getLocation());
			this->destinationPoint = this->navigator.getNextPoint();
			if (this->destinationPoint == this->getLocation())
			{
				this->destinationPoint = this->navigator.getNextPoint();
			}
		}
		
		this->direction = (this->destinationPoint - this->getLocation()).rotation();
	}
}
