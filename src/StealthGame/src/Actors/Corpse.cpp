#include "Actors/Corpse.h"

#include <Components/TransformComponent.h>
#include <Components/RenderComponent.h>
#include <Components/CollisionComponent.h>

Corpse::Corpse(World *world, const Vector2D location, const Rotator rotation) :
	Actor(world)
{
	auto transformComponent = makeAndAddComponent<TransformComponent>();
	transformComponent->setLocation(location);
	transformComponent->setRotation(rotation);
	makeAndAddComponent<RenderComponent>();
	updateActorId("Corpse");
}


Corpse::~Corpse()
{
}
