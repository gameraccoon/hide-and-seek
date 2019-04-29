#include "changetransformcomponentlocationcommand.h"

#include <Components/TransformComponent.h>

ChangeTransformComponentLocationCommand::ChangeTransformComponentLocationCommand(const Entity& entity, const Vector2D& oldLocation, const Vector2D& newLocation)
	: mEntity(entity)
	, mOldLocation(oldLocation)
	, mNewLocation(newLocation)
{
}

void ChangeTransformComponentLocationCommand::doCommand(World* world)
{
	auto [component] = world->getEntityManger().getEntityComponents<TransformComponent>(mEntity);
	if (component)
	{
		component->setLocation(mNewLocation);
	}
}

void ChangeTransformComponentLocationCommand::undoCommand(World* world)
{
	auto [component] = world->getEntityManger().getEntityComponents<TransformComponent>(mEntity);
	if (component)
	{
		component->setLocation(mOldLocation);
	}
}
