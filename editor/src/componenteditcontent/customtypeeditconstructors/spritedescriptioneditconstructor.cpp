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

		Edit<SpriteDescription>::Ptr edit = std::make_shared<Edit<SpriteDescription>>(initialValue);
		Edit<SpriteDescription>::WeakPtr editWeakPtr = edit;
		{
			Edit<std::string>::Ptr editPath = FillEdit<std::string>::Call(layout, "path", initialValue.path);
			editPath->bindOnChange([editWeakPtr](const std::string& /*oldValue*/, const std::string& newValue, bool)
			{
				if (Edit<SpriteDescription>::Ptr edit = editWeakPtr.lock())
				{
					SpriteDescription animDescription = edit->getPreviousValue();
					animDescription.path = newValue;
					edit->transmitValueChange(animDescription);
				}
			});
			edit->addChild(editPath);
		}
		{
			Edit<Vector2D>::Ptr editAnchor = FillEdit<Vector2D>::Call(layout, "anchor", initialValue.params.anchor);
			editAnchor->bindOnChange([editWeakPtr](const Vector2D& /*oldValue*/, const Vector2D& newValue, bool)
			{
				if (Edit<SpriteDescription>::Ptr edit = editWeakPtr.lock())
				{
					SpriteDescription animDescription = edit->getPreviousValue();
					animDescription.params.anchor = newValue;
					edit->transmitValueChange(animDescription);
				}
			});
			edit->addChild(editAnchor);
		}

		{
			Edit<Vector2D>::Ptr editSize = FillEdit<Vector2D>::Call(layout, "size", initialValue.params.size);
			editSize->bindOnChange([editWeakPtr](const Vector2D& /*oldValue*/, const Vector2D& newValue, bool)
			{
				if (Edit<SpriteDescription>::Ptr edit = editWeakPtr.lock())
				{
					SpriteDescription animDescription = edit->getPreviousValue();
					animDescription.params.size = newValue;
					edit->transmitValueChange(animDescription);
				}
			});
			edit->addChild(editSize);
		}

		return edit;
	}
}
