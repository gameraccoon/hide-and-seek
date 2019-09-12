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

		Edit<AnimationDescription>::Ptr edit = std::make_shared<Edit<AnimationDescription>>(initialValue);
		Edit<AnimationDescription>::WeakPtr editWeakPtr = edit;
		{
			Edit<std::string>::Ptr editPath = FillEdit<std::string>::Call(layout, "path", initialValue.path);
			editPath->bindOnChange([editWeakPtr](const std::string& /*oldValue*/, const std::string& newValue, bool)
			{
				if (Edit<AnimationDescription>::Ptr edit = editWeakPtr.lock())
				{
					AnimationDescription animDescription = edit->getPreviousValue();
					animDescription.path = newValue;
					edit->transmitValueChange(animDescription);
				}
			});
			edit->addChild(editPath);
		}
		{
			Edit<std::string>::Ptr editAnimId = FillEdit<std::string>::Call(layout, "animId", initialValue.params.globalAnimId);
			editAnimId->bindOnChange([editWeakPtr](const std::string& /*oldValue*/, const std::string& newValue, bool)
			{
				if (Edit<AnimationDescription>::Ptr edit = editWeakPtr.lock())
				{
					AnimationDescription animDescription = edit->getPreviousValue();
					animDescription.params.globalAnimId = newValue;
					edit->transmitValueChange(animDescription);
				}
			});
			edit->addChild(editAnimId);
		}

		{
			Edit<bool>::Ptr editIsLooped = FillEdit<bool>::Call(layout, "is looped", initialValue.params.isLooped);
			editIsLooped->bindOnChange([editWeakPtr](bool /*oldValue*/, bool newValue, bool)
			{
				if (Edit<AnimationDescription>::Ptr edit = editWeakPtr.lock())
				{
					AnimationDescription animDescription = edit->getPreviousValue();
					animDescription.params.isLooped = newValue;
					edit->transmitValueChange(animDescription);
				}
			});
			edit->addChild(editIsLooped);
		}

		{
			Edit<float>::Ptr editSpeed = FillEdit<float>::Call(layout, "speed", initialValue.params.speed);
			editSpeed->bindOnChange([editWeakPtr](float /*oldValue*/, float newValue, bool)
			{
				if (Edit<AnimationDescription>::Ptr edit = editWeakPtr.lock())
				{
					AnimationDescription animDescription = edit->getPreviousValue();
					animDescription.params.speed = newValue;
					edit->transmitValueChange(animDescription);
				}
			});
			edit->addChild(editSpeed);
		}

		{
			Edit<Vector2D>::Ptr editAnchor = FillEdit<Vector2D>::Call(layout, "anchor", initialValue.spriteParams.anchor);
			editAnchor->bindOnChange([editWeakPtr](const Vector2D& /*oldValue*/, const Vector2D& newValue, bool)
			{
				if (Edit<AnimationDescription>::Ptr edit = editWeakPtr.lock())
				{
					AnimationDescription animDescription = edit->getPreviousValue();
					animDescription.spriteParams.anchor = newValue;
					edit->transmitValueChange(animDescription);
				}
			});
			edit->addChild(editAnchor);
		}

		{
			Edit<Vector2D>::Ptr editSize = FillEdit<Vector2D>::Call(layout, "size", initialValue.spriteParams.size);
			editSize->bindOnChange([editWeakPtr](const Vector2D& /*oldValue*/, const Vector2D& newValue, bool)
			{
				if (Edit<AnimationDescription>::Ptr edit = editWeakPtr.lock())
				{
					AnimationDescription animDescription = edit->getPreviousValue();
					animDescription.spriteParams.size = newValue;
					edit->transmitValueChange(animDescription);
				}
			});
			edit->addChild(editSize);
		}

		return edit;
	}
}
