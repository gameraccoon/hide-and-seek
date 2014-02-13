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
	void startShooting(Vector2D currentLocation, Vector2D targetLocation);
	void stopShooting();
	void changeDirection(Vector2D targetLocation);
	void setOwnerWorld(World *world);
private:
	World *ownerWorld;
};

#endif
