#include "Actors/Hero.h"

#include <AI/PlayerRole.h>

#include <Modules/Collide.h>
#include <Components/TransformComponent.h>
#include <Components/CameraComponent.h>

Hero::Hero(World *world, Vector2D location)
	: Body(world, location)
	, mStep(ZERO_VECTOR)
{
	auto cameraComponent = makeAndAddComponent<CameraComponent>();
	cameraComponent->setTransformComponent(getSingleComponent<TransformComponent>());
	world->setMainCamera(cameraComponent);

	mSpeed = 1.f;

	updateActorId("Hero");

	setFraction(Fraction::GoodGuys);
	
	mRole = std::make_unique<PlayerRole>(world, this);
}

void Hero::move(Vector2D step)
{
	mStep += step;
}

void Hero::update(float deltatime)
{
	Vector2D step =  mStep * deltatime;

	// if actor's path is free
	if (!Collide::isWillCollide(this, getOwnerWorld(), step))
	{
		auto transform = getSingleComponent<TransformComponent>();
		if (transform != nullptr)
		{
			// accept new position of the man
			transform->setLocation(transform->getLocation() + step);
		}
	}

	step = ZERO_VECTOR;

	Body::update(deltatime);
}
