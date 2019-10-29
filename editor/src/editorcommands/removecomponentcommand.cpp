#include "removecomponentcommand.h"

#include <QtWidgets/qcombobox.h>

#include "Base/Debug/Assert.h"
#include "ECS/ComponentFactory.h"
#include "GameData/World.h"

RemoveComponentCommand::RemoveComponentCommand(Entity entity, const QString& typeName, ComponentFactory* factory)
	: mEntity(entity)
	, mComponentTypeName(typeName)
	, mComponentFactory(factory)
{
}

bool RemoveComponentCommand::doCommand(World* world)
{
	StringID typeName = static_cast<StringID>(mComponentTypeName.toStdString());

	if (mSerializedComponent.empty())
	{
		std::vector<BaseComponent*> components = world->getEntityManager().getAllEntityComponents(mEntity);

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

	world->getEntityManager().removeComponent(
		mEntity,
		mComponentFactory->getTypeIDFromString(typeName).value()
	);
	return false;
}

bool RemoveComponentCommand::undoCommand(World* world)
{
	StringID componentTypenameID = static_cast<StringID>(mComponentTypeName.toStdString());
	BaseComponent* component = mComponentFactory->createComponent(componentTypenameID);

	component->fromJson(mSerializedComponent);

	world->getEntityManager().addComponent(
		mEntity,
		component,
		mComponentFactory->getTypeIDFromString(componentTypenameID).value()
	);
	return false;
}

EditorCommand::EffectType RemoveComponentCommand::getEffectType()
{
	return EffectType::Components;
}
