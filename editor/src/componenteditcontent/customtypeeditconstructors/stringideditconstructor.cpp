#include "../typeseditconstructor.h"

#include <string>

#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QHBoxLayout>

#include "Base/String/StringID.h"

namespace TypesEditConstructor
{
	template<>
	Edit<StringID>::Ptr FillEdit<StringID>::Call(QLayout* layout, const QString& label, const StringID& initialValue)
	{
		FillLabel(layout, label);

		QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<StringID>::Ptr edit = std::make_shared<Edit<StringID>>(initialValue);
		Edit<StringID>::WeakPtr editWeakPtr = edit;

		Edit<std::string>::Ptr editX = FillEdit<std::string>::Call(innerLayout, "value", ID_TO_STR(initialValue));
		editX->bindOnChange([editWeakPtr](std::string /*oldValue*/, std::string newValue, bool)
		{
			if (Edit<StringID>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(STR_TO_ID(newValue));
			}
		});
		edit->addChild(editX);

		innerLayout->addStretch();
		QWidget* container = new QWidget();
		container->setLayout(innerLayout);
		layout->addWidget(container);
		return edit;
	}
}
