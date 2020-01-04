#include "addentitygroupcommand.h"

#include <QtWidgets/qcombobox.h>

#include "GameData/World.h"
#include "GameData/Components/TransformComponent.generated.h"

AddEntityGroupCommand::AddEntityGroupCommand(const std::vector<nlohmann::json>& entities, ComponentFactory* factory, const Vector2D& shift)
	: EditorCommand(EffectType::Entities)
	, mEntities(entities)
	, mComponentFactory(factory)
	, mShift(shift)
{
}

void AddEntityGroupCommand::doCommand(World* world)
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
}

void AddEntityGroupCommand::undoCommand(World* world)
{
	for (Entity entity : mCreatedEntities)
	{
		world->getEntityManager().removeEntity(entity);
	}
}
