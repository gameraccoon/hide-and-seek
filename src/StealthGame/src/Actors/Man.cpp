#include "Actors/Man.h"

#include "AI/AiRole.h"

#include <Modules/Collide.h>


Man::Man(World *world, Vector2D location, Vector2D scale, Rotator ) : Body(world, location)
{
	setType(ActorType::Living);

	mSpeed = 50.0f;

	updateActorId("Man");

	setFraction(Fraction::BadGuys);

	if (mRole != nullptr)
		delete mRole;
	mRole = new AiRole(world, this);
}

void Man::update(float deltatime)
{
	if (mTempLocation == getLocation())
	{
		findNextPathPoint();
	}

	float stepSize = mSpeed * deltatime;

	if (stepSize < (mTempLocation - getLocation()).size())
	{
		Vector2D step = (mTempLocation - getLocation()).ort() * stepSize;

		// if actor's path is free
		if (!Collide::isWillCollide(this, getOwnerWorld(), step))
		{
			// accept new position of the man
			setLocation(getLocation() + step);
		}
		else
		{
			findNextPathPoint();
		}
	}
	else
	{
		setLocation(mTempLocation);
	}

	// use superclass method
	Body::update(deltatime);
}

void Man::hit(IActor *instigator, float damageValue, Vector2D impulse)
{
	Body::hit(instigator, damageValue, impulse);
}
