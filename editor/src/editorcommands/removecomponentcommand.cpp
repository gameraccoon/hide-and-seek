#include "removecomponentcommand.h"

#include <QtWidgets/qcombobox.h>

#include "ECS/ComponentFactory.h"
#include "GameData/World.h"

#include "src/editorutils/componentreferenceutils.h"

RemoveComponentCommand::RemoveComponentCommand(const ComponentSourceReference& source, StringID typeName, ComponentFactory* factory)
	: EditorCommand(EffectType::Components)
	, mSource(source)
	, mComponentTypeName(typeName)
	, mComponentFactory(factory)
{
}

void RemoveComponentCommand::doCommand(World* world)
{
	if (mSerializedComponent.empty())
	{
		std::vector<BaseComponent*> components = Utils::GetComponents(mSource, world);

		auto it = std::find_if(components.begin(), components.end(), [typeName = mComponentTypeName](BaseComponent* component)
		{
			return component->getComponentTypeName() == typeName;
		});

		if (it == components.end())
		{
			return;
		}

		(*it)->toJson(mSerializedComponent);
	}

	Utils::RemoveComponent(
		mSource,
		mComponentTypeName,
		world,
		*mComponentFactory
	);
}

void RemoveComponentCommand::undoCommand(World* world)
{
	BaseComponent* component = mComponentFactory->createComponent(mComponentTypeName);

	component->fromJson(mSerializedComponent);

	Utils::AddComponent(
		mSource,
		component,
		world,
		*mComponentFactory
	);
}
