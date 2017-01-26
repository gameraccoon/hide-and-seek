#pragma once

#include <Core/Vector2D.h>
#include <Core/World.h>
#include <Core/Actor.h>
#include <Inventory/InventoryItem.h>
#include <Inventory/Weapon.h>
#include <Actors/Body.h>

class Man:public Body
{
public:
	/** Initialization of a new man standing at a given point */
	Man(World *world, Vector2D location, Vector2D scale, Rotator rotation);

	virtual ~Man() override = default;

	/** Process moving and other actions of the man */
	virtual void update(float deltatime) override;

	/** Try to take some damage to the man =) */
	virtual void hit(IActor *instigator, float damageValue,Vector2D impulse) override;
};
