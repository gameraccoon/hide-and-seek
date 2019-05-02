#include "transformcomponenteditfactory.h"

#include <QWidget>
#include <QLayout>

#include "typeseditconstructor.h"
#include "src/editorcommands/changetransformcomponentlocationcommand.h"
#include "src/editorcommands/changetransformcomponentrotationcommand.h"
#include "src/editorcommands/editorcommandsstack.h"

#include <Debug/Log.h>
#include <Components/TransformComponent.generated.h>

void TransformComponentEditData::fillContent(QLayout* layout, const Entity& entity, const BaseComponent* component, EditorCommandsStack& commandStack, World* world)
{
	const TransformComponent* transform = dynamic_cast<const TransformComponent*>(component);

	mLocationEdit = TypesEditConstructor::FillVector2DEdit(layout, "location", transform->getLocation());
	mLocationEdit->bindOnChange([entity, &commandStack, world](const Vector2D& oldValue, const Vector2D& newValue)
	{
		commandStack.executeNewCommand<ChangeTransformComponentLocationCommand>(world,
			entity,
			oldValue,
			newValue);
	});

	mRotationEdit = TypesEditConstructor::FillFloatEdit(layout, "rotation", transform->getRotation().getValue());
	mRotationEdit->bindOnChange([entity, &commandStack, world](float oldValue, float newValue)
	{
		commandStack.executeNewCommand<ChangeTransformComponentRotationCommand>(world,
			entity,
			oldValue,
			newValue);
	});
}

std::shared_ptr<EditData> TransformComponentEditFactory::getEditData()
{
	return std::make_shared<TransformComponentEditData>();
}
