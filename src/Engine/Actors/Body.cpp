#include "Body.h"

#include "../Modules/ActorFactory.h"

Body::Body(World *world, Vector2D location) : Actor(world, location, Rotator(0.0f)),
	navigator(world),
	size(32.0f, 32.0f),
	tempLocation(location)
{
	this->setType(ActorType::Living);

	this->speed = 12.0f;
	
	this->healthValue = 10000.0f;

	Hull geometry;
	geometry.points.insert(geometry.points.end(), -this->size / 2);
	geometry.points.insert(geometry.points.end(), (this->size / 2).mirrorV());
	geometry.points.insert(geometry.points.end(), this->size / 2);
	geometry.points.insert(geometry.points.end(), (this->size / 2).mirrorH());
	geometry.generate();
	this->setGeometry(geometry);
	
	this->updateCollision();

	this->armedWeapon = nullptr;

	this->followingTarget = nullptr;
	this->movingToLocation = nullptr;

	this->role = nullptr;
}

Body::~Body(void)
{
	if (this->role != nullptr)
	{
		delete this->role;
		this->role = nullptr;
	}
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

float Body::getHealthValue()
{
	return this->healthValue;
}

void Body::clearTargets()
{
	if (this->followingTarget != nullptr)
	{
		delete this->followingTarget;
		this->followingTarget = nullptr;
	}

	if (this->movingToLocation != nullptr)
	{
		delete this->movingToLocation;
		this->movingToLocation = nullptr;
	}
}

void Body::updateCollision()
{
	this->setColideBox(BoundingBox(this->getLocation() - this->size/2, this->getLocation() + this->size/2));
}

void Body::startShoting(Vector2D targetLocation)
{
	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->startShooting(this->getLocation(), (targetLocation - this->getLocation()).rotation());
	}
}

void Body::stopShoting()
{
	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->stopShooting();
	}
}

void Body::giveWeapon(Weapon *weap)
{
	this->armedWeapon = weap;
	weap->setOwnerWorld(this->getOwnerWorld());
	this->armedWeapon->setEquipped(true);
}

void Body::hit(IActor *instigator, float damageValue, Vector2D impulse)
{
	this->healthValue -= damageValue;
	
	//this->setLocation(this->getLocation() + impulse);

	if (this->healthValue <= 0.0f)
	{
		this->healthValue = 0.0f;
		this->speed = 0.0f;
		ActorFactory::Factory().placeActor("Corpse", this->getOwnerWorld(), this->getLocation(), Vector2D(1.f, 1.f), this->getRotation());
		this->destroy();
	}

	this->role->onTakeDamage(this, damageValue, impulse);
}

void Body::update(float deltatime)
{
	this->role->update(deltatime);

	this->look();

	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->update(deltatime);
	}

	Actor::update(deltatime);
}

void Body::onUpdateLocation()
{
	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->setLocation(this->getLocation());
	}
}

void Body::onUpdateRotation()
{
	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->changeDirection(this->getRotation());
	}
}

void Body::look()
{
	for (auto actor : this->getOwnerWorld()->allActors)
	{
		if (actor->getType() == ActorType::Living && actor != this && (this->getLocation() - actor->getLocation()).size() < 60)
		{
			role->onSeeEnemy(actor);
			break;
		}
	}
}

void Body::findNextPathPoint()
{
	if (this->followingTarget != nullptr)
	{
		RayTrace ray(this->getOwnerWorld(), this->getLocation(), this->followingTarget->getLocation());
		IActor* tracedActor = ray.trace();

		if (tracedActor == this->followingTarget || tracedActor == nullptr)
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

		this->setRotation((this->tempLocation - this->getLocation()).rotation());
	}
}
