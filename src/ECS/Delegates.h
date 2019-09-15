#pragma once

#include <vector>
#include <functional>

#include <nlohmann/json_fwd.hpp>

template <typename... Args>
class SinglecastDelegate
{
public:
	using FunctionType = std::function<void(Args...)>;

public:
	void assign(FunctionType fn)
	{
		mFunction = fn;
	}

	void clear()
	{
		mFunction = nullptr;
	}

	void callSafe(Args... args)
	{
		if (mFunction)
		{
			mFunction(std::forward<Args>(args)...);
		}
	}

	void callUnsafe(Args... args)
	{
		mFunction(std::forward<Args>(args)...);
	}

private:
	FunctionType mFunction;
};

namespace Delegates
{
	class Handle
	{
	public:
		Handle() = default;
		explicit Handle(int index)
			: mIndex(index)
		{
		}

		bool operator ==(const Handle& b) const
		{
			return mIndex == b.mIndex;
		}

		bool operator !=(const Handle& b) const { return !(*this == b); }

	private:
		int mIndex = -1;
	};
}

template <typename... Args>
class MulticastDelegate
{
public:
	using FunctionType = std::function<void(Args...)>;

public:
	Delegates::Handle bind(FunctionType fn)
	{
		if (fn)
		{
			Delegates::Handle newHandle(mNextFunctionId++);
			mFunctions.emplace_back(newHandle, fn);
			return newHandle;
		}
		return Delegates::Handle();
	}

	void unbind(Delegates::Handle handle)
	{
		mFunctions.erase(
			std::remove_if(
				mFunctions.begin(),
				mFunctions.end(),
				[handle](FunctionData& val){
					return val.handle == handle;
				}
			),
			mFunctions.end()
		);
	}

	void broadcast(Args... args)
	{
		for (auto fnData : mFunctions)
		{
			fnData.fn(std::forward<Args>(args)...);
		}
	}

	void clear()
	{
		mFunctions.clear();
	}

private:
	struct FunctionData
	{
		FunctionData(Delegates::Handle handle, FunctionType fn)
			: handle(handle)
			, fn(fn)
		{}

		Delegates::Handle handle;
		FunctionType fn;
	};

private:
	std::vector<FunctionData> mFunctions;
	int mNextFunctionId = 0;
};
