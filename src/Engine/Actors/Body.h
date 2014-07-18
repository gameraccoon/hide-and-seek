#ifndef BODY_H
#define BODY_H

#include "../Core/Vector2D.h"
#include "../Core/World.h"
#include "../Modules/PathFinder.h"
#include "../AI/Role.h"
#include "../AI/IBody.h"
#include "../Inventory/InventoryItem.h"
#include "../Inventory/Weapon.h"
#include "../Actors/Actor.h"

class Body:public Actor, public IBody
{
public:
	enum class Fraction
	{
		BadGuys,
		GoodGuys
	};

	/** Initialization of a new man standing at a given point */
	Body(World *world, Vector2D location);

	virtual ~Body();

	virtual void moveTo(Vector2D step) override;
	virtual void follow(const IActor *target) override;

	virtual float getHealthValue() override;

	virtual void giveWeapon(Weapon *weap) override;
	
	/** Start shoting if the man have a weapon */
	virtual void startShoting(Vector2D targetLocation) override;
	virtual void stopShoting() override;

	virtual void update(float deltatime) override;
	
	virtual void hit(IActor *instigator, float damageValue, Vector2D impulse) override;

	Fraction getFraction();
protected:
	/* events */
	virtual void onUpdateLocation() override;
	virtual void onUpdateRotation() override;
	/** */
	void findNextPathPoint();
	void clearTargets();
	/** */
	void look();
	/** */
	void setFraction(Fraction newFraction);
	/** */
	bool canSeeEnemy(const Body *enemy) const;
	/**  */
	PathFinder navigator;
	/** Body moving speed in Px/s */
	float speed;
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
	IActor const *followingTarget;
	/** The location that the man moving to */
	Vector2D *movingToLocation;
	/** Some intermediate location that the man is moveing to now */
	Vector2D tempLocation;
	/** */
	Fraction fraction;
};

#endif
