#pragma once

#include <vector>
#include <functional>

#include <nlohmann/json_fwd.hpp>

template <typename... Args>
class SinglecastDelegate
{
public:
	typedef std::function<void(Args...)> FunctionType;

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

template <typename... Args>
class MulticastDelegate
{
public:
	typedef std::function<void(Args...)> FunctionType;
	typedef int HandleType;

public:
	HandleType bind(FunctionType fn)
	{
		if (fn)
		{
			mFunctions.emplace_back(mNextFunctionId++, fn);
			return mNextFunctionId - 1;
		}
		return -1;
	}

	void unbind(HandleType handle)
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
		FunctionData(HandleType handle, FunctionType fn)
			: handle(handle)
			, fn(fn)
		{}

		HandleType handle;
		FunctionType fn;
	};

private:
	std::vector<FunctionData> mFunctions;
	int mNextFunctionId = 0;
};
