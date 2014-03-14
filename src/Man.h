#ifndef MAN_H
#define MAN_H

#include "../src/Globals.h"
#include "../src/Vector2D.h"
#include "../src/World.h"
#include "../src/Actor.h"
#include "../src/InventoryItem.h"
#include "../src/Weapon.h"
#include "../src/PathFinder.h"
#include "../src/DummyMan.h"

class Man:public DummyMan
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
