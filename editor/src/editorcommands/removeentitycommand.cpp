#include "removeentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include "ECS/Serialization/ComponentSerializersHolder.h"

#include "GameData/World.h"

RemoveEntityCommand::RemoveEntityCommand(Entity entity, const ComponentSerializersHolder& serializerHolder)
	: EditorCommand(EffectType::Entities)
	, mEntity(entity)
	, mComponentSerializerHolder(serializerHolder)
{
}

void RemoveEntityCommand::doCommand(World* world)
{
	if (mSerializedComponents.empty())
	{
		world->getEntityManager().getPrefabFromEntity(mSerializedComponents, mEntity, mComponentSerializerHolder.jsonSerializer);
	}

	world->getEntityManager().removeEntity(mEntity);
}

void RemoveEntityCommand::undoCommand(World* world)
{
	world->getEntityManager().insertEntityUnsafe(mEntity);
	world->getEntityManager().applyPrefabToExistentEntity(mSerializedComponents, mEntity, mComponentSerializerHolder);
}
