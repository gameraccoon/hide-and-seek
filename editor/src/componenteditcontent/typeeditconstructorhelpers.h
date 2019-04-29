#ifndef TYPEEDITCONSTRUCTORHELPERS_H
#define TYPEEDITCONSTRUCTORHELPERS_H

#include <memory>
#include <functional>
#include <QObject>

namespace TypesEditConstructor
{
	class BaseEdit
	{
	public:
		typedef std::shared_ptr<BaseEdit> Ptr;
	};

	template<typename T>
	class Edit : public BaseEdit
	{
	public:
		typedef std::shared_ptr<Edit> Ptr;
		typedef std::weak_ptr<Edit> WeakPtr;
		typedef std::function<void(const T&, const T&)> OnChangeFn;

	public:
		Edit(T initialValue)
			: mPrevValue(std::forward<T>(initialValue))
		{
		}

		~Edit()
		{
			delete mOwner;
		}

		void bindOnChange(OnChangeFn onChange)
		{
			mOnChange = onChange;
		}

		void transmitValueChange(T newValue)
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

		void addChild(const QString& childName, BaseEdit::Ptr child)
		{
			mChildObjects.try_emplace(childName, child);
		}

		QObject* getOwner() { return mOwner; }
		T getPreviousValue() const { return mPrevValue; }

		Edit(const Edit&) = delete;
		Edit& operator=(const Edit&) = delete;

	private:
		T mPrevValue;
		OnChangeFn mOnChange;
		QObject* mOwner = new QObject();
		std::map<QString, BaseEdit::Ptr> mChildObjects;
	};
}

#endif // TYPEEDITCONSTRUCTORHELPERS_H
