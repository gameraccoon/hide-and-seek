#ifndef WEAPON_H
#define WEAPON_H

#include "../../Engine/Core/Vector2D.h"
#include "../../Engine/Core/World.h"
#include "../../Engine/Structures/InventoryItem.h"
#include "../Actors/Bullet.h"

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
