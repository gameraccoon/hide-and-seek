#include "Actors/Body.h"

#include "Modules/ActorFactory.h"

#include "Actors/LightEmitter.h"

#include <Components/MovementComponent.h>
#include <Components/LightComponent.h>
#include <Components/CollisionComponent.h>
#include <Components/RenderComponent.h>

#include <cmath>

Body::Body(World *world, Vector2D location)
	: Actor(world)
	, mNavigator(world)
	, mTempLocation(location)
{
	auto movementComponent = makeAndAddComponent<MovementComponent>();
	movementComponent->overrideOnUpdateLocationCallback([this](){this->onUpdateLocation();});
	movementComponent->overrideOnUpdateRotationCallback([this](){this->onUpdateRotation();});
	makeAndAddComponent<CollisionComponent>();
	auto renderComponent = makeAndAddComponent<RenderComponent>();
	renderComponent->setMovementComponent(movementComponent);

	setType(ActorType::Living);

	mSpeed = 12.0f;
	
	mHealthValue = 10000.0f;

//	setOriginalSize(Vector2D(32.0f, 32.0f));
//	Hull geometry;
//	geometry.type = Hull::Type::Circular;
//	geometry.setRadius(16.0f);
//	setGeometry(geometry);
	
//	updateGeometry();

	mArmedWeapon = nullptr;

	mFollowingTarget = nullptr;
	mMovingToLocation = nullptr;

	mRole = nullptr;
}

Body::~Body()
{
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
		auto movementComponent = getSingleComponent<MovementComponent>();
		if (movementComponent != nullptr)
		{
			Vector2D location = movementComponent->getLocation();
			mArmedWeapon->startShooting(location, (targetLocation - location).rotation());
		}
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
		auto movementComponent = getSingleComponent<MovementComponent>();
		if (movementComponent != nullptr)
		{
			Vector2D location = movementComponent->getLocation();
			Rotator rotation = movementComponent->getRotation();
			ActorFactory::Factory().spawnActor("Corpse", getOwnerWorld(), location, Vector2D(1.f, 1.f), rotation);
		}
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
		auto movementComponent = getSingleComponent<MovementComponent>();
		if (movementComponent != nullptr)
		{
			mArmedWeapon->setLocation(movementComponent->getLocation());
		}
	}
}

void Body::onUpdateRotation()
{
	if (mArmedWeapon != nullptr)
	{
		auto movementComponent = getSingleComponent<MovementComponent>();
		if (movementComponent != nullptr)
		{
			mArmedWeapon->changeDirection(movementComponent->getRotation());
		}
	}
}

void Body::look()
{
	for (const auto& actor : getOwnerWorld()->getAllActors())
	{
		// if actor is a human or a creature and actor isn't this body
		if (actor->getType() == ActorType::Living && actor.get() != this)
		{
			Body *body = dynamic_cast<Body*>(actor.get());
			// if actor is a body and it is an enemy
			if (body != nullptr && body->getFraction() == Fraction::GoodGuys)
			{
				if (canSeeEnemy(body))
				{
					mRole->onSeeEnemy(actor.get());
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

	auto movementComponent = getSingleComponent<MovementComponent>();
	if (movementComponent == nullptr)
	{
		return false;
	}
	auto enemyMovementComponent = getSingleComponent<MovementComponent>();
	if (enemyMovementComponent == nullptr)
	{
		return false;
	}

	Vector2D location = movementComponent->getLocation();
	Rotator rotation = movementComponent->getRotation();
	Vector2D enemyLocation = enemyMovementComponent->getLocation();

	// if enemy farther than viewDistance
	if ((location - enemyLocation).size() > viewDistance)
	{
		return false;
	}

	Rotator angle = (enemyLocation - location).rotation();

	// if actor isn't on the front
	if (abs((angle - rotation).getValue()) > angleOfView)
	{
		return false;
	}

	IActor *tracedActor = RayTrace::trace(getOwnerWorld(), location, enemyLocation);
	if (tracedActor != enemy)
	{
		return false;
	}

	float lightness = 0.f;

	for (const auto& lightComponent : getOwnerWorld()->getComponents<LightComponent>())
	{
		MovementComponent::WeakPtr movementComponent = lightComponent->getMovementComponent();
		if (MovementComponent::Ptr lockedMovementComponent = movementComponent.lock())
		{
			Vector2D lightLocation = lockedMovementComponent->getLocation();
			if ((lightLocation - enemyLocation).size() < lightComponent->getBrightness() * 512)
			{
				IActor *tracedActor2 = RayTrace::trace(getOwnerWorld(), lightLocation, enemyLocation);
				if (tracedActor2 == enemy)
				{
					float sz = (lightLocation - enemyLocation).size();

					if (sz < 0.1f)
						sz = 0.1f;

					lightness += (lightComponent->getBrightness() * 256) / sz;
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
	auto movementComponent = getSingleComponent<MovementComponent>();
	if (movementComponent == nullptr)
	{
		return;
	}
	auto targetMovementComponent = getSingleComponent<MovementComponent>();
	if (targetMovementComponent == nullptr)
	{
		return;
	}

	Vector2D location = movementComponent->getLocation();
	Vector2D targetLocation = targetMovementComponent->getLocation();

	if (mFollowingTarget != nullptr)
	{
		IActor* tracedActor = RayTrace::trace(getOwnerWorld(), location, targetLocation);

		if (tracedActor == mFollowingTarget)
		{
			mTempLocation = targetLocation;
		}
		else
		{
			mNavigator.createNewPath(location, targetLocation);
			mTempLocation = mNavigator.getNextPoint();
			if (mTempLocation == location)
			{
				mTempLocation = mNavigator.getNextPoint();
			}
		}

		movementComponent->setRotation((mTempLocation - location).rotation());
	}
	else if (mMovingToLocation != nullptr)
	{
		IActor* tracedActor = RayTrace::trace(getOwnerWorld(), location, *mMovingToLocation);

		if (tracedActor == nullptr)
		{
			mTempLocation = *mMovingToLocation;
		}
		else
		{
			mNavigator.createNewPath(location, *mMovingToLocation);
			mTempLocation = mNavigator.getNextPoint();
			if (mTempLocation == location)
			{
				mTempLocation = mNavigator.getNextPoint();
			}
		}

		movementComponent->setRotation((mTempLocation - location).rotation());
	}
}

void Body::setFraction(Fraction newFraction)
{
	mFraction = newFraction;
}
