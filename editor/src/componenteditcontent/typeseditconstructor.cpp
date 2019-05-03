#include "typeseditconstructor.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

#include <Core/Vector2D.h>
#include <Structures/Hull.h>

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

	template<>
	Edit<float>::Ptr FillEdit<float>(QLayout* layout, const QString& label, const float& initialValue)
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

	template<>
	Edit<bool>::Ptr FillEdit<bool>(QLayout* layout, const QString& label, const bool& initialValue)
	{
		return std::make_shared<Edit<bool>>(initialValue);
	}

	template<>
	Edit<std::string>::Ptr FillEdit<std::string>(QLayout* layout, const QString& label, const std::string& initialValue)
	{
		FillLabel(layout, label);

		QLineEdit* stringEdit = new QLineEdit();
		stringEdit->setText(QString::fromStdString(initialValue));

		Edit<std::string>::Ptr edit = std::make_shared<Edit<std::string>>(initialValue);
		Edit<std::string>::WeakPtr editWeakPtr = edit;

		stringEdit->connect(stringEdit, &QLineEdit::textChanged, edit->getOwner(), [editWeakPtr](const QString& newValue)
		{
			if (Edit<std::string>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(newValue.toStdString());
			}
		});

		layout->addWidget(stringEdit);
		return edit;
	}

	template<>
	Edit<Rotator>::Ptr FillEdit<Rotator>(QLayout* layout, const QString& label, const Rotator& initialValue)
	{
		FillLabel(layout, label);

		QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<Rotator>::Ptr edit = std::make_shared<Edit<Rotator>>(initialValue);
		Edit<Rotator>::WeakPtr editWeakPtr = edit;

		Edit<float>::Ptr editX = FillEdit<float>(innerLayout, "value", initialValue.getValue() * RAD_TO_DEG);
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

	template<>
	Edit<Vector2D>::Ptr FillEdit<Vector2D>(QLayout* layout, const QString& label, const Vector2D& initialValue)
	{
		FillLabel(layout, label);

		QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<Vector2D>::Ptr edit = std::make_shared<Edit<Vector2D>>(initialValue);
		Edit<Vector2D>::WeakPtr editWeakPtr = edit;

		Edit<float>::Ptr editX = FillEdit<float>(innerLayout, "x", initialValue.x);
		editX->bindOnChange([editWeakPtr](float /*oldValue*/, float newValue)
		{
			if (Edit<Vector2D>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(Vector2D(newValue, edit->getPreviousValue().y));
			}
		});
		edit->addChild("x", editX);

		Edit<float>::Ptr editY = FillEdit<float>(innerLayout, "y", initialValue.y);
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

	template<>
	Edit<Hull>::Ptr FillEdit<Hull>(QLayout* layout, const QString& label, const Hull& initialValue)
	{
		return std::make_shared<Edit<Hull>>(initialValue);
	}
}
