#include "typeseditconstructor.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

static const float RAD_TO_DEG = 180.0f / PI;
static const float DEG_TO_RAD = 1.0f / RAD_TO_DEG;

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
		floatEdit->setText(QString::number(static_cast<double>(initialValue)));

		Edit<float>::Ptr edit = std::make_shared<Edit<float>>(initialValue);
		Edit<float>::WeakPtr editWeakPtr = edit;

		floatEdit->connect(floatEdit, &QLineEdit::textChanged, edit->getOwner(), [editWeakPtr](const QString& newValueStr)
		{
			if (Edit<float>::Ptr edit = editWeakPtr.lock())
			{
				bool ok;
				float newValue = newValueStr.toFloat(&ok);
				if (ok)
				{
					edit->transmitValueChange(newValue);
				}
			}
		});

		layout->addWidget(floatEdit);
		return edit;
	}

	Edit<Rotator>::Ptr FillRotatorEdit(QLayout* layout, const QString& label, const Rotator& initialValue)
	{
		FillLabel(layout, label);

		QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<Rotator>::Ptr edit = std::make_shared<Edit<Rotator>>(initialValue);
		Edit<Rotator>::WeakPtr editWeakPtr = edit;

		Edit<float>::Ptr editX = FillFloatEdit(innerLayout, "value", initialValue.getValue() * RAD_TO_DEG);
		editX->bindOnChange([editWeakPtr](float /*oldValue*/, float newValue)
		{
			if (Edit<Rotator>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(Rotator(newValue * DEG_TO_RAD));
			}
		});
		edit->addChild("value", editX);

		innerLayout->addStretch();
		QWidget* container = new QWidget();
		container->setLayout(innerLayout);
		layout->addWidget(container);
		return edit;
	}

	Edit<Vector2D>::Ptr FillVector2DEdit(QLayout* layout, const QString& label, const Vector2D& initialValue)
	{
		FillLabel(layout, label);

		QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<Vector2D>::Ptr edit = std::make_shared<Edit<Vector2D>>(initialValue);
		Edit<Vector2D>::WeakPtr editWeakPtr = edit;

		Edit<float>::Ptr editX = FillFloatEdit(innerLayout, "x", initialValue.x);
		editX->bindOnChange([editWeakPtr](float /*oldValue*/, float newValue)
		{
			if (Edit<Vector2D>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(Vector2D(newValue, edit->getPreviousValue().y));
			}
		});
		edit->addChild("x", editX);

		Edit<float>::Ptr editY = FillFloatEdit(innerLayout, "y", initialValue.y);
		editY->bindOnChange([editWeakPtr](float /*oldValue*/, float newValue)
		{
			if (Edit<Vector2D>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(Vector2D(edit->getPreviousValue().x, newValue));
			}
		});
		edit->addChild("y", editY);

		innerLayout->addStretch();
		QWidget* container = new QWidget();
		container->setLayout(innerLayout);
		layout->addWidget(container);
		return edit;
	}
}
