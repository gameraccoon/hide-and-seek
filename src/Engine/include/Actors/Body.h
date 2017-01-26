#pragma once

#include "Core/Vector2D.h"
#include "Core/World.h"
#include "Modules/PathFinder.h"
#include "AI/Role.h"
#include "AI/IBody.h"
#include "Inventory/InventoryItem.h"
#include "Inventory/Weapon.h"
#include "Core/Actor.h"

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
	PathFinder mNavigator;
	/** Body moving speed in Px/s */
	float mSpeed;
	/** */
	float mHealthValue;
	/** Inventory */
	std::vector<InventoryItem*> mInventory;
	/** */
	Weapon *mArmedWeapon;
	/** Is we shooting */
	bool mIsShooting;
	/** AI */
	Role *mRole;
	/** The target that the man follows */
	IActor const *mFollowingTarget;
	/** The location that the man moving to */
	Vector2D *mMovingToLocation;
	/** Some intermediate location that the man is moveing to now */
	Vector2D mTempLocation;
	/** */
	Fraction mFraction;
};
