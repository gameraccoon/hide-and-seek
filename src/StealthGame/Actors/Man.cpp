#include "Man.h"

#include "../AI/AiRole.h"

#include <Engine/Modules/Collide.h>


Man::Man(World *world, Vector2D location, Vector2D scale, Rotator rotation) : Body(world, location)
{
	this->setType(ActorType::Living);

	this->speed = 50.0f;

	this->updateActorId("Man");

	this->setFraction(Fraction::BadGuys);

	if (this->role != nullptr)
		delete this->role;
	this->role = new AiRole(world, this);
}

Man::~Man(void)
{
}

void Man::update(float deltatime)
{
	if (this->tempLocation == this->getLocation())
	{
		this->findNextPathPoint();
	}

	float stepSize = this->speed * deltatime;

	if (stepSize < (this->tempLocation - this->getLocation()).size())
	{
		Vector2D step = (this->tempLocation - this->getLocation()).ort() * stepSize;
		bool bFree = true;

		// if actor's path is free
		if (!Collide::isWillCollide(this, this->getOwnerWorld(), step))
		{
			// accept new position of the man
			this->setLocation(this->getLocation() + step);
		}
		else
		{
			findNextPathPoint();
		}
	}
	else
	{
		this->setLocation(this->tempLocation);
	}

	// use superclass method
	Body::update(deltatime);
}

void Man::hit(IActor *instigator, float damageValue, Vector2D impulse)
{
	Body::hit(instigator, damageValue, impulse);
}