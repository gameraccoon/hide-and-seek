#include "Actors/Body.h"

#include "Modules/ActorFactory.h"

#include "Actors/LightEmitter.h"

#include <cmath>

Body::Body(World *world, Vector2D location)
	: Actor(world, location, Rotator(0.0f))
	, mNavigator(world)
	, mTempLocation(location)
{
	setType(ActorType::Living);

	mSpeed = 12.0f;

	setOriginalSize(Vector2D(32.0f, 32.0f));
	
	mHealthValue = 10000.0f;

	Hull geometry;
	geometry.type = Hull::Type::Circular;
	geometry.setRadius(16.0f);
	setGeometry(geometry);
	
	updateGeometry();

	mArmedWeapon = nullptr;

	mFollowingTarget = nullptr;
	mMovingToLocation = nullptr;

	mRole = nullptr;
}

Body::~Body()
{
	if (mRole != nullptr)
	{
		delete mRole;
		mRole = nullptr;
	}
}

void Body::moveTo(Vector2D step)
{
	clearTargets();

	mMovingToLocation = new Vector2D(step);
	mFollowingTarget = nullptr;
}

void Body::follow(const IActor *target)
{
	clearTargets();

	mFollowingTarget = target;
	mMovingToLocation = nullptr;
}

float Body::getHealthValue()
{
	return mHealthValue;
}

void Body::clearTargets()
{
	if (mFollowingTarget != nullptr)
	{
		delete mFollowingTarget;
		mFollowingTarget = nullptr;
	}

	if (mMovingToLocation != nullptr)
	{
		delete mMovingToLocation;
		mMovingToLocation = nullptr;
	}
}

void Body::startShoting(Vector2D targetLocation)
{
	if (mArmedWeapon != nullptr)
	{
		mArmedWeapon->startShooting(getLocation(), (targetLocation - getLocation()).rotation());
	}
}

void Body::stopShoting()
{
	if (mArmedWeapon != nullptr)
	{
		mArmedWeapon->stopShooting();
	}
}

void Body::giveWeapon(Weapon *weap)
{
	mArmedWeapon = weap;
	weap->setOwnerWorld(getOwnerWorld());
	mArmedWeapon->setEquipped(true);
}

void Body::hit(IActor *, float damageValue, Vector2D impulse)
{
	mHealthValue -= damageValue;
	
	//setLocation(getLocation() + impulse);

	if (mHealthValue <= 0.0f)
	{
		mHealthValue = 0.0f;
		mSpeed = 0.0f;
		ActorFactory::Factory().placeActor("Corpse", getOwnerWorld(), getLocation(), Vector2D(1.f, 1.f), getRotation());
		destroy();
	}
	else
	{
		mRole->onTakeDamage(this, damageValue, impulse);
	}
}

void Body::update(float deltatime)
{
	mRole->update(deltatime);

	look();

	if (mArmedWeapon != nullptr)
	{
		mArmedWeapon->update(deltatime);
	}

	Actor::update(deltatime);
}

Body::Fraction Body::getFraction()
{
	return mFraction;
}

void Body::onUpdateLocation()
{
	if (mArmedWeapon != nullptr)
	{
		mArmedWeapon->setLocation(getLocation());
	}
}

void Body::onUpdateRotation()
{
	if (mArmedWeapon != nullptr)
	{
		mArmedWeapon->changeDirection(getRotation());
	}
}

void Body::look()
{
	for (const auto actor : getOwnerWorld()->getAllActors())
	{
		// if actor is a human or a creature and actor isn't this body
		if (actor->getType() == ActorType::Living && actor != this)
		{
			Body *body = dynamic_cast<Body*>(actor);
			// if actor is a body and it is an enemy
			if (body != nullptr && body->getFraction() == Fraction::GoodGuys)
			{
				if (canSeeEnemy(body))
				{
					mRole->onSeeEnemy(actor);
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
	if ((getLocation() - enemy->getLocation()).size() > viewDistance)
	{
		return false;
	}

	Vector2D loc = enemy->getLocation();
	Rotator angle = (loc - getLocation()).rotation();

	// if actor isn't on the front
	if (abs((angle - getRotation()).getValue()) > angleOfView)
	{
		return false;
	}

	IActor *tracedActor = RayTrace::trace(getOwnerWorld(), getLocation(), enemy->getLocation());
	if (tracedActor != enemy)
	{
		return false;
	}

	float lightness = 0.f;

	// ToDo: need to refactor next fragment
	for (const auto actor : getOwnerWorld()->getAllActors())
	{
		if (actor->getType() == ActorType::Light)
		{
			LightEmitter *light = dynamic_cast<LightEmitter*>(actor);
			if (light != nullptr)
			{
				if ((light->getLocation() - enemy->getLocation()).size() < light->getBrightness() * 512)
				{
					IActor *tracedActor2 = RayTrace::trace(getOwnerWorld(), light->getLocation(), enemy->getLocation());
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
	if (mFollowingTarget != nullptr)
	{
		IActor* tracedActor = RayTrace::trace(getOwnerWorld(), getLocation(), mFollowingTarget->getLocation());

		if (tracedActor == mFollowingTarget)
		{
			mTempLocation = mFollowingTarget->getLocation();
		}
		else
		{
			mNavigator.createNewPath(getLocation(), mFollowingTarget->getLocation());
			mTempLocation = mNavigator.getNextPoint();
			if (mTempLocation == getLocation())
			{
				mTempLocation = mNavigator.getNextPoint();
			}
		}

		setRotation((mTempLocation - getLocation()).rotation());
	}
	else if (mMovingToLocation != nullptr)
	{
		IActor* tracedActor = RayTrace::trace(getOwnerWorld(), getLocation(), *mMovingToLocation);

		if (tracedActor == nullptr)
		{
			mTempLocation = *mMovingToLocation;
		}
		else
		{
			mNavigator.createNewPath(getLocation(), *mMovingToLocation);
			mTempLocation = mNavigator.getNextPoint();
			if (mTempLocation == getLocation())
			{
				mTempLocation = mNavigator.getNextPoint();
			}
		}

		setRotation((mTempLocation - getLocation()).rotation());
	}
}

void Body::setFraction(Fraction newFraction)
{
	mFraction = newFraction;
}
