#ifndef TYPEEDITCONSTRUCTORHELPERS_H
#define TYPEEDITCONSTRUCTORHELPERS_H

#include <memory>
#include <functional>
#include <QObject>

namespace TypesEditConstructor
{
	template<typename T>
	class Edit
	{
	public:
		typedef std::shared_ptr<Edit> Ptr;

	public:
		Edit(T initialValue)
		: mPrevValue(std::forward<T>(initialValue))
		{
		}

		void bindOnChange(std::function<void(const T&, const T&)> onChange)
		{
		mOnChange = onChange;
		}

		void changeValue(T newValue)
		{
			if (newValue != mPrevValue)
			{
				if (mOnChange)
				{
					mOnChange(mPrevValue, std::forward<T>(newValue));
				}
				mPrevValue = newValue;
			}
		}

		QObject* getOwner() { return owner; }
		T getPreviousValue() const { return mPrevValue; }

	private:
		T mPrevValue;
		std::function<void(const T&, const T&)> mOnChange;
		QObject* owner = new QObject();
	};
}

#endif // TYPEEDITCONSTRUCTORHELPERS_H
