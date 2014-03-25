#ifndef MAN_H
#define MAN_H

#include "../../Engine/Core/Vector2D.h"
#include "../../Engine/Core/World.h"
#include "../../Engine/Actors/Actor.h"
#include "../../Engine/Structures/InventoryItem.h"
#include "../../Engine/Modules/PathFinder.h"
#include "../../Engine/Inventory/Weapon.h"
#include "../../Engine/Actors/Body.h"

class Man:public Body
{
public:
	/** Initialization of a new man standing at a given point */
	Man(World *ownerWorld, Vector2D location);

	~Man();

	/** Process moving and other actions of the man */
	virtual void update(float deltatime);

	/** Try to take some damage to the man =) */
	virtual void takeDamage(float damageValue,Vector2D impulse);
protected:
	/** */
	void findNextPathPoint();
	/**  */
	PathFinder navigator;
	/** Point to which this man aspire */
	Vector2D destinationPoint;
	/** */
	IActor* target;
};

#endif
