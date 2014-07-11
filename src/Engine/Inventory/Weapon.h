#ifndef WEAPON_H
#define WEAPON_H

#include "../Core/Vector2D.h"
#include "../Core/World.h"
#include "../Actors/Bullet.h"

#include "InventoryItem.h"

class Weapon : public InventoryItem
{
public:
	Weapon(void);
	~Weapon(void);
	void startShooting(Vector2D location, Rotator direction);
	void stopShooting();
	void changeDirection(Rotator newDirection);
	void setLocation(Vector2D newLocation);
	void update(float deltaTime);
	void setOwnerWorld(World *world);
private:
	World *ownerWorld;
	bool isFiring;
	float fireInterval;
	float lastFireTime;
	float startFireDelay;
	Vector2D location;
	Rotator direction;
};

#endif
