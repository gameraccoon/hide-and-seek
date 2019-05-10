#include "removecomponentcommand.h"

#include <QtWidgets/qcombobox.h>

#include "Core/World.h"
#include "Debug/Assert.h"
#include "Modules/ComponentFactory.h"

RemoveComponentCommand::RemoveComponentCommand(Entity entity, const QString& typeName, ComponentFactory* factory)
	: mEntity(entity)
	, mComponentTypeName(typeName)
	, mComponentFactory(factory)
{
}

bool RemoveComponentCommand::doCommand(World* world)
{
	std::string typeName = mComponentTypeName.toStdString();

	if (mSerializedComponent.empty())
	{
		std::vector<BaseComponent*> components = world->getEntityManger().getAllEntityComponents(mEntity);

		auto it = std::find_if(components.begin(), components.end(), [&typeName](BaseComponent* component)
		{
			return component->getComponentTypeName() == typeName;
		});

		if (it == components.end())
		{
			return false;
		}

		(*it)->toJson(mSerializedComponent);
	}

	world->getEntityManger().removeComponent(
		mEntity,
		mComponentFactory->getTypeIDFromString(typeName).value()
	);
	return false;
}

bool RemoveComponentCommand::undoCommand(World* world)
{
	BaseComponent* component = mComponentFactory->createComponent(mComponentTypeName.toStdString());

	component->fromJson(mSerializedComponent);

	world->getEntityManger().addComponent(
		mEntity,
		component,
		mComponentFactory->getTypeIDFromString(mComponentTypeName.toStdString()).value()
	);

	return false;
}
