#include "src/componenteditcontent/customtypeeditconstructors/customtypeeditconstructors.h"

#include <string>

#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QHBoxLayout>

namespace TypesEditConstructor
{
	template<>
	Edit<Entity>::Ptr FillEdit<Entity>::Call(QLayout* layout, const QString& label, const Entity& initialValue)
	{
		FillLabel(layout, label);

		QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<Entity>::Ptr edit = std::make_shared<Edit<Entity>>(initialValue);
		Edit<Entity>::WeakPtr editWeakPtr = edit;

		Edit<Entity::EntityID>::Ptr editAngle = FillEdit<Entity::EntityID>::Call(innerLayout, "id", initialValue.getID());
		editAngle->bindOnChange([editWeakPtr](float /*oldValue*/, Entity::EntityID newValue, bool)
		{
			if (Edit<Entity>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(Entity(newValue));
			}
		});
		edit->addChild(editAngle);

		innerLayout->addStretch();
		QWidget* container = new QWidget();
		container->setLayout(innerLayout);
		layout->addWidget(container);
		return edit;
	}
}
