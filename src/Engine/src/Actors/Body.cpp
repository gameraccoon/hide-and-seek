#include "Actors/Body.h"

#include "Modules/ActorFactory.h"

#include "Actors/LightEmitter.h"

#include <Components/TransformComponent.h>
#include <Components/LightComponent.h>
#include <Components/CollisionComponent.h>
#include <Components/RenderComponent.h>

#include <cmath>

Body::Body(World *world, Vector2D location)
	: Actor(world)
	, mNavigator(world)
	, mTempLocation(location)
{
	auto transformComponent = makeAndAddComponent<TransformComponent>();
	makeAndAddComponent<CollisionComponent>();
	auto renderComponent = makeAndAddComponent<RenderComponent>();
	renderComponent->setTransformComponent(transformComponent);
	renderComponent->setTexturePath("resources/textures/hero.png");

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
		auto transformComponent = getSingleComponent<TransformComponent>();
		if (transformComponent != nullptr)
		{
			Vector2D location = transformComponent->getLocation();
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
		auto transformComponent = getSingleComponent<TransformComponent>();
		if (transformComponent != nullptr)
		{
			Vector2D location = transformComponent->getLocation();
			Rotator rotation = transformComponent->getRotation();
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

	auto transformComponent = getSingleComponent<TransformComponent>();
	if (transformComponent == nullptr)
	{
		return false;
	}
	auto enemyTransformComponent = getSingleComponent<TransformComponent>();
	if (enemyTransformComponent == nullptr)
	{
		return false;
	}

	Vector2D location = transformComponent->getLocation();
	Rotator rotation = transformComponent->getRotation();
	Vector2D enemyLocation = enemyTransformComponent->getLocation();

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
		TransformComponent::WeakPtr transformComponent = lightComponent->getTransformComponent();
		if (TransformComponent::Ptr lockedTransformComponent = transformComponent.lock())
		{
			Vector2D lightLocation = lockedTransformComponent->getLocation();
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
	auto transformComponent = getSingleComponent<TransformComponent>();
	if (transformComponent == nullptr)
	{
		return;
	}
	auto targetTransformComponent = getSingleComponent<TransformComponent>();
	if (targetTransformComponent == nullptr)
	{
		return;
	}

	Vector2D location = transformComponent->getLocation();
	Vector2D targetLocation = targetTransformComponent->getLocation();

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

		transformComponent->setRotation((mTempLocation - location).rotation());
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

		transformComponent->setRotation((mTempLocation - location).rotation());
	}
}

void Body::setFraction(Fraction newFraction)
{
	mFraction = newFraction;
}
