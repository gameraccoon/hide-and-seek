#pragma once

#include <Core/Vector2D.h>

class World;

#include "InventoryItem.h"

class Weapon : public InventoryItem
{
public:
	Weapon();
	~Weapon();
	void startShooting(const Vector2D& location, const Rotator& direction);
	void stopShooting();
	void changeDirection(const Rotator& newDirection);
	void setLocation(const Vector2D& newLocation);
	void update(float deltaTime);
	void setOwnerWorld(World *world);

private:
	World *mOwnerWorld;
	bool mIsFiring;
	float mFireInterval;
	float mLastFireTime;
	float mStartFireDelay;
	Vector2D mLocation;
	Rotator mDirection;
};
