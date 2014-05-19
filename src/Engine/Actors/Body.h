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

	virtual void moveTo(Vector2D step) override;
	virtual void follow(IActor *target);

	virtual float getHealthValue() override;

	virtual void giveWeapon(Weapon *weap) override;
	
	/** Start shoting if the man have a weapon */
	virtual void startShoting(Vector2D targetLocation) override;
	virtual void stopShoting() override;

	virtual void update(float deltatime) override;
	
	virtual void hit(IActor *instigator, float damageValue, Vector2D impulse) override;
protected:
	/** */
	virtual void updateCollision() override;
	/** */
	void findNextPathPoint();
	void clearTargets();
	/** */
	void look();
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
