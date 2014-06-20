#include "Body.h"

#include "../Modules/ActorFactory.h"

#include "../Actors/LightEmitter.h"

Body::Body(World *world, Vector2D location) : Actor(world, location, Rotator(0.0f)),
	navigator(world),
	size(32.0f, 32.0f),
	tempLocation(location)
{
	this->setType(ActorType::Living);

	this->speed = 12.0f;
	
	this->healthValue = 10000.0f;

	Hull geometry;
	geometry.points.insert(geometry.points.end(), -this->size / 2);
	geometry.points.insert(geometry.points.end(), (this->size / 2).mirrorV());
	geometry.points.insert(geometry.points.end(), this->size / 2);
	geometry.points.insert(geometry.points.end(), (this->size / 2).mirrorH());
	geometry.generate();
	this->setGeometry(geometry);
	
	this->updateCollision();

	this->armedWeapon = nullptr;

	this->followingTarget = nullptr;
	this->movingToLocation = nullptr;

	this->role = nullptr;
}

Body::~Body(void)
{
	if (this->role != nullptr)
	{
		delete this->role;
		this->role = nullptr;
	}
}

void Body::moveTo(Vector2D step)
{
	this->clearTargets();

	this->movingToLocation = new Vector2D(step);
	this->followingTarget = nullptr;
}

void Body::follow(const IActor *target)
{
	this->clearTargets();

	this->followingTarget = target;
	this->movingToLocation = nullptr;
}

float Body::getHealthValue()
{
	return this->healthValue;
}

void Body::clearTargets()
{
	if (this->followingTarget != nullptr)
	{
		delete this->followingTarget;
		this->followingTarget = nullptr;
	}

	if (this->movingToLocation != nullptr)
	{
		delete this->movingToLocation;
		this->movingToLocation = nullptr;
	}
}

void Body::updateCollision()
{
	this->setColideBox(BoundingBox(this->getLocation() - this->size/2, this->getLocation() + this->size/2));
}

void Body::startShoting(Vector2D targetLocation)
{
	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->startShooting(this->getLocation(), (targetLocation - this->getLocation()).rotation());
	}
}

void Body::stopShoting()
{
	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->stopShooting();
	}
}

void Body::giveWeapon(Weapon *weap)
{
	this->armedWeapon = weap;
	weap->setOwnerWorld(this->getOwnerWorld());
	this->armedWeapon->setEquipped(true);
}

void Body::hit(IActor *instigator, float damageValue, Vector2D impulse)
{
	this->healthValue -= damageValue;
	
	//this->setLocation(this->getLocation() + impulse);

	if (this->healthValue <= 0.0f)
	{
		this->healthValue = 0.0f;
		this->speed = 0.0f;
		ActorFactory::Factory().placeActor("Corpse", this->getOwnerWorld(), this->getLocation(), Vector2D(1.f, 1.f), this->getRotation());
		this->destroy();
	}
	else
	{
		this->role->onTakeDamage(this, damageValue, impulse);
	}
}

void Body::update(float deltatime)
{
	this->role->update(deltatime);

	this->look();

	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->update(deltatime);
	}

	Actor::update(deltatime);
}

Body::Fraction Body::getFraction()
{
	return this->fraction;
}

void Body::onUpdateLocation()
{
	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->setLocation(this->getLocation());
	}
}

void Body::onUpdateRotation()
{
	if (this->armedWeapon != nullptr)
	{
		this->armedWeapon->changeDirection(this->getRotation());
	}
}

void Body::look()
{
	for (auto actor : this->getOwnerWorld()->allActors)
	{
		// if actor is a human or a creature and actor isn't this body
		if (actor->getType() == ActorType::Living && actor != this)
		{
			Body *body = dynamic_cast<Body*>(actor);
			// if actor is a body and it is an enemy
			if (body != nullptr && body->getFraction() == Fraction::GoodGuys)
			{
				if (this->canSeeEnemy(body))
				{
					role->onSeeEnemy(actor);
					break;
				}
			}
		}
	}
}

bool Body::canSeeEnemy(const Body *enemy) const
{
	const float angleOfView = PI/4;
	const float viewDistance = 400.f;
	const float attentiveness = 1.5f;

	// if enemy farther than viewDistance
	if ((this->getLocation() - enemy->getLocation()).size() > viewDistance)
	{
		return false;
	}

	Vector2D loc = enemy->getLocation();
	Rotator angle = (loc - this->getLocation()).rotation();

	// if actor isn't on the front
	if (abs((angle - this->getRotation()).getValue()) > angleOfView)
	{
		return false;
	}

	IActor *tracedActor = RayTrace::trace(this->getOwnerWorld(), this->getLocation(), enemy->getLocation());
	if (tracedActor != enemy)
	{
		return false;
	}

	float lightness = 0.f;

	// ToDo: need to refactor next fragment
	for (auto actor : this->getOwnerWorld()->allActors)
	{
		if (actor->getType() == ActorType::Light)
		{
			LightEmitter *light = dynamic_cast<LightEmitter*>(actor);
			if (light != nullptr)
			{
				if ((light->getLocation() - enemy->getLocation()).size() < light->getBrightness() * 512)
				{
					IActor *tracedActor2 = RayTrace::trace(this->getOwnerWorld(), light->getLocation(), enemy->getLocation());
					if (tracedActor2 == enemy)
					{
						float sz = (light->getLocation() - enemy->getLocation()).size();
						
						if (sz < 0.1f)
							sz = 0.1f;
						
						lightness += (light->getBrightness() * 256) / sz;
					}
				}
			}
		}
	}

	float visibility = lightness;

	if (visibility > attentiveness)
	{
		return true;
	}

	return false;
}

void Body::findNextPathPoint()
{
	if (this->followingTarget != nullptr)
	{
		IActor* tracedActor = RayTrace::trace(this->getOwnerWorld(), this->getLocation(), this->followingTarget->getLocation());

		if (tracedActor == this->followingTarget)
		{
			this->tempLocation = followingTarget->getLocation();
		}
		else
		{
			this->navigator.createNewPath(this->getLocation(), this->followingTarget->getLocation());
			this->tempLocation = this->navigator.getNextPoint();
			if (this->tempLocation == this->getLocation())
			{
				this->tempLocation = this->navigator.getNextPoint();
			}
		}

		this->setRotation((this->tempLocation - this->getLocation()).rotation());
	}
	else if (this->movingToLocation != nullptr)
	{
		IActor* tracedActor = RayTrace::trace(this->getOwnerWorld(), this->getLocation(), *this->movingToLocation);

		if (tracedActor == nullptr)
		{
			this->tempLocation = *this->movingToLocation;
		}
		else
		{
			this->navigator.createNewPath(this->getLocation(), *this->movingToLocation);
			this->tempLocation = this->navigator.getNextPoint();
			if (this->tempLocation == this->getLocation())
			{
				this->tempLocation = this->navigator.getNextPoint();
			}
		}

		this->setRotation((this->tempLocation - this->getLocation()).rotation());
	}
}

void Body::setFraction(Fraction newFraction)
{
	this->fraction = newFraction;
}