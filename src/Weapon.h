#ifndef WEAPON_H
#define WEAPON_H

#include "../src/InventoryItem.h"
#include "../src/Vector2D.h"
#include "../src/World.h"

class Weapon : public InventoryItem
{
public:
	Weapon(World *ownerWorld);
	~Weapon(void);
	void StartShooting(Vector2D targetLocation);
	void StopShooting();
	void ChangeDirection(Vector2D targetLocation);
private:
	World *OwnerWorld;
};

#endif