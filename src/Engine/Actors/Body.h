#ifndef BODY_H
#define BODY_H

#include "../Core/Vector2D.h"
#include "../Core/World.h"
#include "../Actors/Actor.h"
#include "../Structures/InventoryItem.h"
#include "../../Engine/Modules/PathFinder.h"
#include "../Inventory/Weapon.h"
#include "../AI/Role.h"
#include "../AI/IBody.h"

class Body:public Actor, public IBody
{
public:
	/** Initialization of a new man standing at a given point */
	Body(World *world, Vector2D location);

	virtual ~Body();

	void moveTo(Vector2D step);
	void follow(IActor *target);

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
	/** */
	void findNextPathPoint();
	void clearTargets();
	/**  */
	PathFinder navigator;
	/** Body moving speed in Px/s */
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
	/** The target that the man follows */
	IActor *followingTarget;
	/** The location that the man moving to */
	Vector2D *movingToLocation;
	/** Some intermediate location that the man is moveing to now */
	Vector2D tempLocation;
};

#endif
