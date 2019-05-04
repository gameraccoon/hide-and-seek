#include "typeseditconstructor.h"

#include <limits>
#include <string>

#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPushButton>

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

		auto dv = new QDoubleValidator(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max(), 10);
		dv->setNotation(QDoubleValidator::StandardNotation);
		floatEdit->setValidator(dv);

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
		FillLabel(layout, label);

		QCheckBox* checkbox = new QCheckBox();
		checkbox->setChecked(initialValue);

		Edit<bool>::Ptr edit = std::make_shared<Edit<bool>>(initialValue);
		Edit<bool>::WeakPtr editWeakPtr = edit;

		checkbox->connect(checkbox, &QCheckBox::stateChanged, edit->getOwner(), [editWeakPtr](int newValue)
		{
			if (Edit<bool>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(static_cast<bool>(newValue));
			}
		});

		layout->addWidget(checkbox);
		return edit;
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

		Edit<float>::Ptr editAngle = FillEdit<float>(innerLayout, "angle", initialValue.getValue() * RAD_TO_DEG);
		editAngle->bindOnChange([editWeakPtr](float /*oldValue*/, float newValue, bool)
		{
			if (Edit<Rotator>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(Rotator(newValue * DEG_TO_RAD));
			}
		});
		edit->addChild(editAngle);

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
		editX->bindOnChange([editWeakPtr](float /*oldValue*/, float newValue, bool)
		{
			if (Edit<Vector2D>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(Vector2D(newValue, edit->getPreviousValue().y));
			}
		});
		edit->addChild(editX);

		Edit<float>::Ptr editY = FillEdit<float>(innerLayout, "y", initialValue.y);
		editY->bindOnChange([editWeakPtr](float /*oldValue*/, float newValue, bool)
		{
			if (Edit<Vector2D>::Ptr edit = editWeakPtr.lock())
			{
				edit->transmitValueChange(Vector2D(edit->getPreviousValue().x, newValue));
			}
		});
		edit->addChild(editY);

		innerLayout->addStretch();
		QWidget* container = new QWidget();
		container->setLayout(innerLayout);
		layout->addWidget(container);
		return edit;
	}

	template<>
	Edit<Hull>::Ptr FillEdit<Hull>(QLayout* layout, const QString& label, const Hull& initialValue)
	{
		FillLabel(layout, label);

		Edit<Hull>::Ptr edit = std::make_shared<Edit<Hull>>(initialValue);
		Edit<Hull>::WeakPtr editWeakPtr = edit;

		Edit<float>::Ptr editRadius = FillEdit<float>(layout, "radius", initialValue.getQRadius());
		editRadius->bindOnChange([editWeakPtr](float /*oldValue*/, float newValue, bool)
		{
			if (Edit<Hull>::Ptr edit = editWeakPtr.lock())
			{
				Hull hull = edit->getPreviousValue();
				hull.setRadius(newValue);
				edit->transmitValueChange(hull);
			}
		});
		edit->addChild(editRadius);

		FillLabel(layout, "points");
		int index = 0;
		for (const Vector2D& point : initialValue.points)
		{
			Edit<Vector2D>::Ptr editPoint = FillEdit<Vector2D>(layout, QString("[%1]").arg(index), point);
			editPoint->bindOnChange([editWeakPtr, index](const Vector2D& /*oldValue*/, const Vector2D& newValue, bool)
			{
				if (Edit<Hull>::Ptr edit = editWeakPtr.lock())
				{
					Hull hull = edit->getPreviousValue();
					hull.points[static_cast<size_t>(index)] = newValue;
					edit->transmitValueChange(hull);
				}
			});
			edit->addChild(editPoint);

			QPushButton* removeButton = new QPushButton();
			removeButton->setText("x");
			Edit<bool>::Ptr removePoint = std::make_shared<Edit<bool>>(false);
			removeButton->connect(removeButton, &QPushButton::pressed, edit->getOwner(), [editWeakPtr, index]()
			{
				if (Edit<Hull>::Ptr edit = editWeakPtr.lock())
				{
					Hull hull = edit->getPreviousValue();
					hull.points.erase(hull.points.begin() + index);
					edit->transmitValueChange(hull, true);
				}
			});
			edit->addChild(removePoint);
			layout->addWidget(removeButton);
			index++;
		}

		QPushButton* addButton = new QPushButton();
		addButton->setText("add point");
		Edit<bool>::Ptr addPoint = std::make_shared<Edit<bool>>(false);
		addButton->connect(addButton, &QPushButton::pressed, edit->getOwner(), [editWeakPtr]()
		{
			if (Edit<Hull>::Ptr edit = editWeakPtr.lock())
			{
				Hull hull = edit->getPreviousValue();
				hull.points.push_back(Vector2D(0.0f, 0.0f));
				edit->transmitValueChange(hull, true);
			}
		});
		edit->addChild(addPoint);
		layout->addWidget(addButton);

		return edit;
	}
}
