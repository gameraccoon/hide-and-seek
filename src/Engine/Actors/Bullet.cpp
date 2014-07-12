#include "Bullet.h"


Bullet::Bullet(World *world, Vector2D location, Vector2D scale, Rotator rotation) :
	Actor(world, location, rotation)
{
	this->speed = 10.0f;	
	this->setType(ActorType::Bullet);

	this->speed = 450.0f;

	this->updateActorId("Bullet");
}

Bullet::~Bullet(void)
{
}

void Bullet::update(float deltatime)
{
	Vector2D newLocation = this->getLocation() + deltatime * this->speed * Vector2D(this->getRotation());

	Vector2D traceLocation(ZERO_VECTOR);
	IActor *trasedActor = RayTrace::trace(this->getOwnerWorld(), this->getLocation(), newLocation, &traceLocation);

	// if there nothing to hit
	if (trasedActor == nullptr)
	{
		this->setLocation(newLocation);
	}
	else // bullet is hiting some actor
	{
		trasedActor->hit(this, 10.f, Vector2D(this->getRotation()) * this->speed * 0.01f);
		this->speed = 0.0f;
		this->destroy();
	}

	// bullet will be destroyed after 10 second
	if (this->getLifetime() > 10.f)
	{
		this->destroy();
	}

	Actor::update(deltatime);
}

void Bullet::updateCollision()
{
}

void Bullet::updateActorId(std::string classId)
{
	this->setClassId(classId);
	this->setActorId("some" + classId);
}
