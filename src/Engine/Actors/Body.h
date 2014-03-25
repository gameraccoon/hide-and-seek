#ifndef BODY_H
#define BODY_H

#include "../Core/Vector2D.h"
#include "../Core/World.h"
#include "../Actors/Actor.h"
#include "../Structures/InventoryItem.h"
#include "../Inventory/Weapon.h"
#include "../AI/Role.h"

class Weapon;

class Body:public Actor
{
public:
	/** Initialization of a new man standing at a given point */
	Body(World *ownerWorld, Vector2D location);

	virtual ~Body();

	/** Say that we want to move the man on this step */
	virtual void move(Vector2D step);

	void giveWeapon(Weapon *weap);
	
	/** Start shoting if the man have a weapon */
	void startShoting(Vector2D targetLocation);
	void stopShoting();

	void update(float deltatime);

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
	/** AI */
	Role *role;
};

#endif
