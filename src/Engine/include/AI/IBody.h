#pragma once

#include "../Inventory/Weapon.h"
#include "../Actors/SoundVolume.h"
#include "../Core/Vector2D.h"
#include "../Core/IActor.h"


class IBody
{
public:
	virtual ~IBody() = default;
	/** Say that the body must go to the location */
	virtual void moveTo(Vector2D location) = 0;
	/**
	 * Say that the body must follow the target
	 * @param target target those the body follows. It's constant that
	 * helps prevent a direct change of the target.
	 */
	virtual void follow(const IActor *target) = 0;

	virtual float getHealthValue() = 0;

	virtual void giveWeapon(Weapon *weap) = 0;

	/** Start shoting if the man have a weapon */
	virtual void startShoting(Vector2D targetLocation) = 0;
	virtual void stopShoting() = 0;
};
