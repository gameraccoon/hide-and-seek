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
	void startShooting(Vector2D currentLocation, Vector2D targetLocation);
	void stopShooting();
	void changeDirection(Vector2D targetLocation);
	void setOwnerWorld(World *world);
private:
	World *ownerWorld;
};

#endif
