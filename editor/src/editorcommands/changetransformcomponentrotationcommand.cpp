#include "changetransformcomponentrotationcommand.h"

#include <Components/TransformComponent.h>

static float RAD_TO_DEG = 180.0f / PI;
static float DEG_TO_RAD = 1.0f / RAD_TO_DEG;

ChangeTransformComponentRotationCommand::ChangeTransformComponentRotationCommand(const Entity& entity, float oldRotation, float newRotation)
	: mEntity(entity)
	, mOldRotation(oldRotation)
	, mNewRotation(newRotation)
{
}

void ChangeTransformComponentRotationCommand::doCommand(World* world)
{
	Rotator rotation(mNewRotation * DEG_TO_RAD);
	auto [component] = world->getEntityManger().getEntityComponents<TransformComponent>(mEntity);
	if (component)
	{
		component->setRotation(rotation);
	}
}

void ChangeTransformComponentRotationCommand::undoCommand(World* world)
{
	Rotator rotation(mOldRotation * DEG_TO_RAD);
	auto [component] = world->getEntityManger().getEntityComponents<TransformComponent>(mEntity);
	if (component)
	{
		component->setRotation(rotation);
	}
}
