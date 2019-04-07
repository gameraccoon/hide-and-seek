#include "Inventory/Weapon.h"

Weapon::Weapon()
	: mLocation(ZERO_VECTOR)
	, mDirection(0.0f)
{
	mFireInterval = 0.1f;
	mLastFireTime = 0.0f;
	mStartFireDelay = 0.01f;
	mIsFiring = false;
}


Weapon::~Weapon()
{
}

void Weapon::startShooting(const Vector2D& location, const Rotator& direction)
{
	mLocation = location;
	mDirection = direction;
	mLastFireTime = mFireInterval - mStartFireDelay;
	mIsFiring = true;
}

void Weapon::stopShooting()
{
	mIsFiring = false;
}

void Weapon::changeDirection(const Rotator& newDirection)
{
	mDirection = newDirection;
}

void Weapon::setLocation(const Vector2D& newLocation)
{
	mLocation = newLocation;
}

void Weapon::update(float deltatime)
{
	mLastFireTime += deltatime;
	if (mIsFiring && mLastFireTime > mFireInterval)
	{
//		ActorFactory::Factory().spawnActor("Bullet", mOwnerWorld, mLocation, Vector2D(1.f, 1.f), mDirection);
		mLastFireTime = 0;
	}
}

void Weapon::setOwnerWorld(World *world)
{
	mOwnerWorld = world;
}
