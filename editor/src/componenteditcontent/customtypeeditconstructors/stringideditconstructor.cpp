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
	Edit<StringID>::Ptr FillEdit<StringID>::Call(QLayout* layout, const QString& label, const StringID& initialValue)
	{
		FillLabel(layout, label);

		QHBoxLayout *innerLayout = HS_NEW QHBoxLayout;

		Edit<StringID>::Ptr edit = std::make_shared<Edit<StringID>>(initialValue);
		Edit<StringID>::WeakPtr editWeakPtr = edit;

		Edit<std::string>::Ptr editX = FillEdit<std::string>::Call(innerLayout, "id", ID_TO_STR(initialValue));
		editX->bindOnChange([editWeakPtr](std::string /*oldValue*/, std::string newValue, bool)
		{
			if (Edit<StringID>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(STR_TO_ID(newValue));
			}
		});
		edit->addChild(editX);

		innerLayout->addStretch();
		QWidget* container = HS_NEW QWidget();
		container->setLayout(innerLayout);
		layout->addWidget(container);
		return edit;
	}
}
