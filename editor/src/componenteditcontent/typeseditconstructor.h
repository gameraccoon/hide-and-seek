#ifndef TYPESEDITCONSTRUCTOR_H
#define TYPESEDITCONSTRUCTOR_H

#include <QObject>
#include <QLayout>
#include <QString>

#include <QComboBox>

#include "typeeditconstructorhelpers.h"

template <typename T>
std::string enum_to_string(T value);

template <typename T>
T string_to_enum(const std::string& value);

template <typename T>
std::vector<T> get_all_enum_values();

template <typename T>
std::vector<std::string> get_all_enum_value_names();

namespace TypesEditConstructor
{
	// helpers
	void FillLabel(QLayout* layout, const QString& label);

	template<typename T, class Enable = void>
	struct FillEdit {
		static typename Edit<T>::Ptr Call(QLayout* layout, const QString& label, const T& initialValue);
	};

	// partial specilization for enums
	template<typename T>
	struct FillEdit<T, typename std::enable_if<std::is_enum<T>::value>::type> {
		static typename Edit<T>::Ptr Call(QLayout* layout, const QString& label, const T& initialValue)
		{
			FillLabel(layout, label);

			QComboBox* stringList = new QComboBox();
			for (const std::string& value : get_all_enum_value_names<T>())
			{
				stringList->addItem(QString::fromStdString(value));
			}
			stringList->setCurrentText(QString::fromStdString(enum_to_string(initialValue)));

			typename Edit<T>::Ptr edit = std::make_shared<Edit<T>>(initialValue);
			typename Edit<T>::WeakPtr editWeakPtr = edit;

			QObject::connect(stringList, &QComboBox::currentTextChanged, edit->getOwner(), [editWeakPtr](const QString& newValue)
			{
				if (typename Edit<T>::Ptr edit = editWeakPtr.lock())
				{
					edit->transmitValueChange(string_to_enum<T>(newValue.toStdString()));
				}
			});

			layout->addWidget(stringList);
			return edit;
		}
	};
}

#endif // TYPESEDITCONSTRUCTOR_H
