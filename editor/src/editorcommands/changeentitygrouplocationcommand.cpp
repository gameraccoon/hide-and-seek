#include "changeentitygrouplocationcommand.h"

#include <Core/World.h>
#include <Components/TransformComponent.generated.h>

ChangeEntityGroupLocationCommand::ChangeEntityGroupLocationCommand(const std::vector<Entity>& entities, const Vector2D& shift)
	: mEntities(entities)
	, mShift(shift)
{
}

bool ChangeEntityGroupLocationCommand::doCommand(World* world)
{
	for (Entity& entity : mEntities)
	{
		auto [component] = world->getEntityManger().getEntityComponents<TransformComponent>(entity);
		if (component)
		{
			component->setLocation(component->getLocation() + mShift);
		}
	}
	return false;
}

bool ChangeEntityGroupLocationCommand::undoCommand(World* world)
{
	for (Entity& entity : mEntities)
	{
		auto [component] = world->getEntityManger().getEntityComponents<TransformComponent>(entity);
		if (component)
		{
			component->setLocation(component->getLocation() - mShift);
		}
	}
	return false;
}
