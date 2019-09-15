#include "addentitygroupcommand.h"

#include <QtWidgets/qcombobox.h>

#include "GameData/World.h"
#include "GameData/Components/TransformComponent.generated.h"

AddEntityGroupCommand::AddEntityGroupCommand(const std::vector<nlohmann::json>& entities, ComponentFactory* factory, const Vector2D& shift)
	: mEntities(entities)
	, mComponentFactory(factory)
	, mShift(shift)
{
}

bool AddEntityGroupCommand::doCommand(World* world)
{
	mCreatedEntities.clear();
	for (const auto& serializedObject : mEntities)
	{
		Entity entity = world->getEntityManager().createPrefabInstance(serializedObject, *mComponentFactory);
		mCreatedEntities.push_back(entity);
		auto [transform] = world->getEntityManager().getEntityComponents<TransformComponent>(entity);
		if (transform)
		{
			transform->setLocation(transform->getLocation() + mShift);
		}
	}
	return false;
}

bool AddEntityGroupCommand::undoCommand(World* world)
{
	for (Entity entity : mCreatedEntities)
	{
		world->getEntityManager().removeEntity(entity);
	}
	return false;
}

EditorCommand::EffectType AddEntityGroupCommand::getEffectType()
{
	return EffectType::Entities;
}
