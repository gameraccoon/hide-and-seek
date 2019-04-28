#ifndef TYPESEDITCONSTRUCTOR_H
#define TYPESEDITCONSTRUCTOR_H

#include <QLayout>
#include <QString>

#include <Core/Vector2D.h>

#include "typeeditconstructorhelpers.h"

namespace TypesEditConstructor
{
	// helpers
	void FillLabel(QLayout* layout, const QString& label);

	// base types
	Edit<float>::Ptr FillFloatEdit(QLayout* layout, const QString& label, float initialValue);

	// complex types
	Edit<Vector2D>::Ptr FillVector2DEdit(QLayout* layout, const QString& label, const Vector2D initialValue);
};

#endif // TYPESEDITCONSTRUCTOR_H
