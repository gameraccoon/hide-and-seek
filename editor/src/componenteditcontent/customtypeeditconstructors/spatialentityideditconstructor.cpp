#include "../typeseditconstructor.h"

#include <string>

#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QHBoxLayout>

#include "GameData/Spatial/SpatialEntityID.h"

namespace TypesEditConstructor
{
	template<>
	Edit<SpatialEntityID>::Ptr FillEdit<SpatialEntityID>::Call(QLayout* layout, const QString& label, const SpatialEntityID& initialValue)
	{
		FillLabel(layout, label);

		QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<SpatialEntityID>::Ptr edit = std::make_shared<Edit<SpatialEntityID>>(initialValue);
		Edit<SpatialEntityID>::WeakPtr editWeakPtr = edit;

		Edit<Entity>::Ptr editX = FillEdit<Entity>::Call(innerLayout, "entity", initialValue.entity.getEntity());
		editX->bindOnChange([editWeakPtr](Entity /*oldValue*/, Entity newValue, bool)
		{
			if (Edit<SpatialEntityID>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(SpatialEntityID(newValue, edit->getPreviousValue().cell));
			}
		});
		edit->addChild(editX);

		Edit<CellPos>::Ptr editY = FillEdit<CellPos>::Call(innerLayout, "cell", initialValue.cell);
		editY->bindOnChange([editWeakPtr](CellPos /*oldValue*/, CellPos newValue, bool)
		{
			if (Edit<SpatialEntityID>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(SpatialEntityID(edit->getPreviousValue().entity.getEntity(), newValue));
			}
		});
		edit->addChild(editY);

		innerLayout->addStretch();
		QWidget* container = new QWidget();
		container->setLayout(innerLayout);
		layout->addWidget(container);
		return edit;
	}
}
