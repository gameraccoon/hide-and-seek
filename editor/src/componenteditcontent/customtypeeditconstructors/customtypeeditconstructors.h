#ifndef TYPESEDITCONSTRUCTOR_H
#define TYPESEDITCONSTRUCTOR_H

#include "../typeeditconstructorhelpers.h"

#include "GameData/Core/Vector2D.h"
#include "GameData/Core/Rotator.h"
#include "GameData/Core/Hull.h"
#include "GameData/Resources/SpriteDescription.h"
#include "GameData/Resources/AnimationDescription.h"

class QLayout;
class QString;

namespace TypesEditConstructor
{
	Edit<Vector2D>::Ptr FillEdit(QLayout* layout, const QString& label, Vector2D initialValue);
	Edit<Rotator>::Ptr FillEdit(QLayout* layout, const QString& label, Rotator initialValue);
	Edit<Hull>::Ptr FillEdit(QLayout* layout, const QString& label, const Hull& initialValue);
	Edit<SpriteDescription>::Ptr FillEdit(QLayout* layout, const QString& label, const SpriteDescription& initialValue);
	Edit<AnimationDescription>::Ptr FillEdit(QLayout* layout, const QString& label, const AnimationDescription& initialValue);
}

#endif // TYPESEDITCONSTRUCTOR_H
