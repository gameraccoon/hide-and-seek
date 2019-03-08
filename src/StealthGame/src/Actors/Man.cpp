#include "Actors/Man.h"

#include "AI/AiRole.h"

#include <Modules/Collide.h>
#include <Components/TransformComponent.h>


Man::Man(World *world, Vector2D location)
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
	auto transformComponent = getSingleComponent<TransformComponent>();
	if (transformComponent != nullptr)
	{
		Vector2D location = transformComponent->getLocation();
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
				transformComponent->setLocation(location + step);
			}
			else
			{
				findNextPathPoint();
			}
		}
		else
		{
			transformComponent->setLocation(mTempLocation);
		}
	}
	// use superclass method
	Body::update(deltatime);
}

void Man::hit(IActor *instigator, float damageValue, Vector2D impulse)
{
	Body::hit(instigator, damageValue, impulse);
}
