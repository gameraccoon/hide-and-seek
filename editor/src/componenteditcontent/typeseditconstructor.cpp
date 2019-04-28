#include "typeseditconstructor.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

namespace TypesEditConstructor
{
	void FillLabel(QLayout* layout, const QString& label)
	{
		QLabel* editLabel = new QLabel();
		editLabel->setText(label + ":");
		layout->addWidget(editLabel);
	}

	Edit<float>::Ptr FillFloatEdit(QLayout* layout, const QString& label, float initialValue)
	{
		FillLabel(layout, label);

		QLineEdit* floatEdit = new QLineEdit();
		Edit<float>::Ptr edit = std::make_shared<Edit<float>>(initialValue);
		floatEdit->connect(floatEdit, &QLineEdit::textChanged, edit->getOwner(), [edit](const QString& newValueStr) {
			bool ok;
			float newValue = newValueStr.toFloat(&ok);
			if (ok)
			{
				edit->changeValue(newValue);
			}
		});
		layout->addWidget(floatEdit);
		return edit;
	}

	Edit<Vector2D>::Ptr FillVector2DEdit(QLayout* layout, const QString& label, const Vector2D initialValue)
	{
		FillLabel(layout, label);

		QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<Vector2D>::Ptr edit = std::make_shared<Edit<Vector2D>>(initialValue);
		Edit<float>::Ptr editX = FillFloatEdit(innerLayout, "x", initialValue.x);
		Edit<float>::Ptr editY = FillFloatEdit(innerLayout, "y", initialValue.y);

		editX->bindOnChange([edit = edit](float /*oldValue*/, float newValue){
			edit->changeValue(Vector2D(newValue, edit->getPreviousValue().y));
		});
		editY->bindOnChange([edit](float /*oldValue*/, float newValue){
			edit->changeValue(Vector2D(edit->getPreviousValue().x, newValue));
		});

		innerLayout->addStretch();
		QWidget* container = new QWidget();
		container->setLayout(innerLayout);
		layout->addWidget(container);
		return edit;
	}
}
