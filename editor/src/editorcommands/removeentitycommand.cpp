#include "removeentitycommand.h"

#include <QtWidgets/qcombobox.h>

#include "Core/World.h"
#include "Debug/Assert.h"
#include "Modules/ComponentFactory.h"

RemoveEntityCommand::RemoveEntityCommand(Entity entity, ComponentFactory* factory)
	: mEntity(entity)
	, mComponentFactory(factory)
{
}

bool RemoveEntityCommand::doCommand(World* world)
{
	if (mSerializedComponents.empty())
	{
		std::vector<BaseComponent*> components = world->getEntityManger().getAllEntityComponents(mEntity);

		mSerializedComponents.resize(components.size());
		for (size_t i = 0; i < components.size(); ++i)
		{
			mSerializedComponents[i].first = components[i]->getComponentTypeName();
			components[i]->toJson(mSerializedComponents[i].second);
		}
	}

	world->getEntityManger().removeEntity(mEntity);
	return false;
}

bool RemoveEntityCommand::undoCommand(World* world)
{
	world->getEntityManger().insertEntityUnsafe(mEntity);

	for (const auto& [typeName, json] : mSerializedComponents)
	{
		BaseComponent* component = mComponentFactory->createComponent(typeName);

		component->fromJson(json);

		world->getEntityManger().addComponent(
			mEntity,
			component,
			mComponentFactory->getTypeIDFromString(typeName).value()
		);
	}
	return false;
}
