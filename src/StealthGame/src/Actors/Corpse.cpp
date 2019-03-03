#include "Actors/Corpse.h"

#include <Components/MovementComponent.h>
#include <Components/RenderComponent.h>
#include <Components/CollisionComponent.h>

Corpse::Corpse(World *world, const Vector2D location, const Rotator rotation) :
	Actor(world)
{
	auto movementComponent = makeAndAddComponent<MovementComponent>();
	movementComponent->setLocation(location);
	movementComponent->setRotation(rotation);
	makeAndAddComponent<RenderComponent>();
	updateActorId("Corpse");
}


Corpse::~Corpse()
{
}
