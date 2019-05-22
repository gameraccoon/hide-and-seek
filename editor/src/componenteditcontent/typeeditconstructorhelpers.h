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
		typedef std::function<void(const T&, const T&, bool)> OnChangeFn;

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

		void transmitValueChange(T newValue, bool forceUpdateLayout = false)
		{
			if (newValue != mPrevValue)
			{
				if (mOnChange)
				{
					mOnChange(mPrevValue, std::forward<T>(newValue), forceUpdateLayout);
				}
				mPrevValue = newValue;
			}
		}

		void addChild(BaseEdit::Ptr child)
		{
			mChildObjects.emplace_back(child);
		}

		QObject* getOwner() { return mOwner; }
		T getPreviousValue() const { return mPrevValue; }

		Edit(const Edit&) = delete;
		Edit& operator=(const Edit&) = delete;

	private:
		T mPrevValue;
		OnChangeFn mOnChange;
		QObject* mOwner = new QObject();
		std::vector<BaseEdit::Ptr> mChildObjects;
	};
}

#endif // TYPEEDITCONSTRUCTORHELPERS_H
