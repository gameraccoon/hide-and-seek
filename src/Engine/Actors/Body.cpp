#include "Body.h"

Body::Body(World *world, Vector2D location) : Actor(world, location, Rotator(0.0f)),
																		size(32.0f, 32.0f),
																		navigator(world),
																		tempLocation(location)
{
	this->type = AT_Living;

	this->speed = 12.0f;
	
	this->healthValue = 100.0f;

	this->geometry.points.insert(this->geometry.points.end(), -this->size / 2);
	this->geometry.points.insert(this->geometry.points.end(), (this->size / 2).mirrorV());
	this->geometry.points.insert(this->geometry.points.end(), this->size / 2);
	this->geometry.points.insert(this->geometry.points.end(), (this->size / 2).mirrorH());
	this->geometry.generate();
	
	this->updateCollision();

	this->role = new Role(world, this);

	this->armedWeapon = NULL;

	this->followingTarget = NULL;
	this->movingToLocation = NULL;
}

Body::~Body(void)
{
	delete this->role;
}

void Body::moveTo(Vector2D step)
{
	this->clearTargets();

	this->movingToLocation = new Vector2D(step);
}

void Body::follow(IActor *target)
{
	this->clearTargets();

	this->followingTarget = target;
}

void Body::clearTargets()
{
	if (this->followingTarget != NULL)
	{
		delete this->followingTarget;
		this->followingTarget = NULL;
	}

	if (this->movingToLocation != NULL)
	{
		delete this->movingToLocation;
		this->movingToLocation = NULL;
	}
}

void Body::updateCollision()
{
	this->colideBox = BoundingBox(this->location - this->size/2, this->location + this->size/2);
}

void Body::startShoting(Vector2D targetLocation)
{
	if (this->armedWeapon != NULL)
	{
		this->armedWeapon->startShooting(this->location ,targetLocation);
	}
}

void Body::stopShoting()
{
	if (this->armedWeapon != NULL)
	{
		this->armedWeapon->stopShooting();
	}
}

void Body::giveWeapon(Weapon *weap)
{
	this->armedWeapon = weap;
	weap->setOwnerWorld(this->ownerWorld);
	this->armedWeapon->setEquipped(true);
}

void Body::takeDamage(float damageValue,Vector2D impulse)
{
	this->healthValue -= damageValue;
	
	this->moveTo(impulse);

	if (this->healthValue <= 0.0f)
	{
		this->healthValue = 0.0f;
		this->speed = 0.0f;
		//this->ownerWorld->spawnActor(new Corpse(this->ownerWorld, this->getLocation(), this->getRotation()));
		this->destroy();
	}
}

void Body::update(float deltatime)
{
	this->role->update(deltatime);

	Actor::update(deltatime);
}

void Body::findNextPathPoint()
{
	if (this->followingTarget != NULL)
	{
		RayTrace ray(this->ownerWorld, this->getLocation(), this->followingTarget->getLocation());
		IActor* tracedActor = ray.trace();

		if (tracedActor == this->followingTarget || tracedActor == NULL)
		{
			this->tempLocation = followingTarget->getLocation();
		}
		else
		{
			this->navigator.createNewPath(this->getLocation(), this->followingTarget->getLocation());
			this->tempLocation = this->navigator.getNextPoint();
			if (this->tempLocation == this->getLocation())
			{
				this->tempLocation = this->navigator.getNextPoint();
			}
		}

		this->direction = (this->tempLocation - this->getLocation()).rotation();
	}
}
