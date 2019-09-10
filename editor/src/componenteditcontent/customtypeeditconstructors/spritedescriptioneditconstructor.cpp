#include "../typeseditconstructor.h"

#include <string>

#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QCheckBox>
#include <QHBoxLayout>

#include "GameData/Resources/SpriteDescription.h"

namespace TypesEditConstructor
{
	template<>
	Edit<SpriteDescription>::Ptr FillEdit<SpriteDescription>::Call(QLayout* layout, const QString& label, const SpriteDescription& initialValue)
	{
		FillLabel(layout, label);

//		QHBoxLayout *innerLayout = new QHBoxLayout;

		Edit<SpriteDescription>::Ptr edit = std::make_shared<Edit<SpriteDescription>>(initialValue);
		Edit<SpriteDescription>::WeakPtr editWeakPtr = edit;

		return edit;
	}
}
