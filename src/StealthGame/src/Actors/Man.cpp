#include "Actors/Man.h"

#include "AI/AiRole.h"

#include <Modules/Collide.h>
#include <Components/MovementComponent.h>


Man::Man(World *world, Vector2D location, Vector2D /*scale*/, Rotator )
	: Body(world, location)
{
	setType(ActorType::Living);

	mSpeed = 50.0f;

	updateActorId("Man");

	setFraction(Fraction::BadGuys);

	mRole = std::make_unique<AiRole>(world, this);
}

void Man::update(float deltatime)
{
	auto movementComponent = getSingleComponent<MovementComponent>();
	if (movementComponent != nullptr)
	{
		Vector2D location = movementComponent->getLocation();
		if (mTempLocation == location)
		{
			findNextPathPoint();
		}

		float stepSize = mSpeed * deltatime;

		Vector2D diffLocation = mTempLocation - location;
		if (stepSize < diffLocation.size())
		{
			Vector2D step = diffLocation.ort() * stepSize;

			// if actor's path is free
			if (!Collide::isWillCollide(this, getOwnerWorld(), step))
			{
				// accept new position of the man
				movementComponent->setLocation(location + step);
			}
			else
			{
				findNextPathPoint();
			}
		}
		else
		{
			movementComponent->setLocation(mTempLocation);
		}
	}
	// use superclass method
	Body::update(deltatime);
}

void Man::hit(IActor *instigator, float damageValue, Vector2D impulse)
{
	Body::hit(instigator, damageValue, impulse);
}
