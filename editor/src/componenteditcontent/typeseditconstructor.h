#ifndef TYPESEDITCONSTRUCTOR_H
#define TYPESEDITCONSTRUCTOR_H

#include <QLayout>
#include <QString>

#include "typeeditconstructorhelpers.h"

namespace TypesEditConstructor
{
	// helpers
	void FillLabel(QLayout* layout, const QString& label);

	template<typename T>
	typename Edit<T>::Ptr FillEdit(QLayout* layout, const QString& label, const T& initialValue);
}

#endif // TYPESEDITCONSTRUCTOR_H
