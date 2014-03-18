#ifndef DUMMYMAN_H
#define DUMMYMAN_H

#include "../../Engine/Core/Vector2D.h"
#include "../../Engine/Core/World.h"
#include "../../Engine/Actors/Actor.h"
#include "../../Engine/Structures/InventoryItem.h"
#include "../Inventory/Weapon.h"

class Weapon;

class DummyMan:public Actor
{
public:
	/** Initialization of a new man standing at a given point */
	DummyMan(World *ownerWorld, Vector2D location);

	virtual ~DummyMan();

	/** Say that we want to move the man on this step */
	virtual void move(Vector2D step);

	void giveWeapon(Weapon *weap);
	
	/** Start shoting if the man have a weapon */
	void startShoting(Vector2D targetLocation);
	void stopShoting();

	/** Try to take some damage to the man =) */
	virtual void takeDamage(float damageValue,Vector2D impulse);
protected:
	/** */
	void updateCollision();
	/** Hero moving speed in Px/s */
	float speed;
	/** */
	Vector2D size;
	/** */
	float healthValue;
	/** Inventory */
	std::vector<InventoryItem*> Inventory;
	/** */
	Weapon *armedWeapon;
	/** Is we shooting */
	bool bShooting;
};

#endif
