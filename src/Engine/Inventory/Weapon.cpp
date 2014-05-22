#include "Weapon.h"


Weapon::Weapon() : location(ZERO_VECTOR), direction(0.0f)
{
	this->fireInterval = 0.1f;
	this->lastFireTime = 0.0f;
	this->startFireDelay = 0.01f;
	this->isFiring = false;
}


Weapon::~Weapon(void)
{
}

void Weapon::startShooting(Vector2D location, Rotator direction)
{
	this->location = location;
	this->direction = direction;
	this->lastFireTime = this->fireInterval - this->startFireDelay;
	this->isFiring = true;
}

void Weapon::stopShooting()
{
	this->isFiring = false;
}

void Weapon::changeDirection(Rotator newDirection)
{
	this->direction = newDirection;
}

void Weapon::setLocation(Vector2D newLocation)
{
	this->location = newLocation;
}

void Weapon::update(float deltatime)
{
	lastFireTime += deltatime;
	if (this->isFiring && this->lastFireTime > this->fireInterval)
	{
		this->ownerWorld->spawnActor(new Bullet(this->ownerWorld, this->location, Vector2D(1.f, 1.f), direction));
		this->lastFireTime = 0;
	}
}

void Weapon::setOwnerWorld(World *world)
{
	this->ownerWorld = world;
}