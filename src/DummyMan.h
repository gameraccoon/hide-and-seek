#ifndef DUMMYMAN_H
#define DUMMYMAN_H

#include "../src/Globals.h"
#include "../src/Vector2D.h"
#include "../src/World.h"
#include "../src/Actor.h"
#include "../src/InventoryItem.h"
#include "../src/Weapon.h"

class Weapon;

class DummyMan:public Actor
{
public:
	/** Initialization of a new man standing at a given point */
	DummyMan(World *ownerWorld, Vector2D location);

	virtual ~DummyMan();

	/** Say that we want to move the man on this step */
	virtual void Move(Vector2D step);

	void Rotate(Rotator newDirection);

	/** Process moving and other actions of the man */
	virtual void Update(float deltaTime) = 0;
	/** Render the actor in the current location */
	void Render(Vector2D shift, Rotator angle);

	void GiveWeapon(Weapon *weap);
	
	/** Start shoting if the man have a weapon */
	void StartShoting(Vector2D targetLocation);
	void StopShoting();

	/** Try to take some damage to the man =) */
	virtual void TakeDamage(float damageValue,Vector2D impulse);
protected:
	/** */
	void UpdateCollision();
	/** Hero moving speed in Px/s */
	float Speed;
	/** */
	Vector2D Size;
	/** Texture of all man's sprites */
	HTEXTURE ManTexture;
	/** Inventory */
	std::vector<InventoryItem*> Inventory;
	/** */
	Weapon *ArmedWeapon;
	/** Is we shooting */
	bool bShooting;
};

#endif