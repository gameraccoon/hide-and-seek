#pragma once

#include <utility>

template<typename CallableType>
class ScopeFinalizer
{
public:
	ScopeFinalizer(CallableType&& finelizeFn)
		: mFinalizeFn(std::move(finelizeFn))
	{}

	ScopeFinalizer(ScopeFinalizer&) = delete;
	ScopeFinalizer& operator=(ScopeFinalizer&) = delete;
	ScopeFinalizer(ScopeFinalizer&&) = delete;
	ScopeFinalizer& operator=(ScopeFinalizer&&) = delete;

	~ScopeFinalizer()
	{
		mFinalizeFn();
	}

private:
	CallableType mFinalizeFn;
};
