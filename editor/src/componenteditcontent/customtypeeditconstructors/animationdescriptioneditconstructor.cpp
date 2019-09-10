#include "../typeseditconstructor.h"

#include <string>

#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QHBoxLayout>

#include "GameData/Resources/AnimationDescription.h"

namespace TypesEditConstructor
{
	template<>
	Edit<AnimationDescription>::Ptr FillEdit<AnimationDescription>::Call(QLayout* layout, const QString& label, const AnimationDescription& initialValue)
	{
		FillLabel(layout, label);

		//QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<AnimationDescription>::Ptr edit = std::make_shared<Edit<AnimationDescription>>(initialValue);

		return edit;
	}
}
