#ifndef WEAPON_H
#define WEAPON_H

#include "../src/InventoryItem.h"
#include "../src/Vector2D.h"
#include "../src/World.h"
#include "../src/Bullet.h"

class Weapon : public InventoryItem
{
public:
	Weapon(void);
	~Weapon(void);
	void StartShooting(Vector2D currentLocation, Vector2D targetLocation);
	void StopShooting();
	void ChangeDirection(Vector2D targetLocation);
	void SetOwnerWorld(World *world);
private:
	World *OwnerWorld;
};

#endif