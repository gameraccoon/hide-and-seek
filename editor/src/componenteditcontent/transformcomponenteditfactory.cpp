#include "transformcomponenteditfactory.h"

#include <QWidget>
#include <QLayout>

#include "typeseditconstructor.h"

#include <Debug/Log.h>
#include <Components/TransformComponent.h>

QWidget* TransformComponentEditData::newContentWidget(const BaseComponent* component)
{
	QVBoxLayout *layout = new QVBoxLayout;

	const TransformComponent* transform = dynamic_cast<const TransformComponent*>(component);

	TypesEditConstructor::Edit<Vector2D>::Ptr transformEdit = TypesEditConstructor::FillVector2DEdit(layout, "location", transform->getLocation());
	TypesEditConstructor::Edit<float>::Ptr rotationEdit = TypesEditConstructor::FillFloatEdit(layout, "rotation", transform->getRotation().getValue());

	transformEdit->bindOnChange([](const Vector2D& oldValue, const Vector2D& newValue)
	{
		LogInfo("test (%f, %f) (%f, %f)", oldValue.x, oldValue.y, newValue.x, newValue.y);
	});

	rotationEdit->bindOnChange([](float oldValue, float newValue)
	{
		LogInfo("test2 %f %f", oldValue, newValue);
	});

	layout->addStretch();
	QWidget* container = new QWidget();
	container->setLayout(layout);
	return container;
}

std::shared_ptr<EditData> TransformComponentEditFactory::getEditData()
{
	return std::make_shared<TransformComponentEditData>();
}
