#pragma once

#include <gtest/gtest.h>

inline void EnableFailOnAssert() noexcept
{
#ifdef DEBUG
	GlobalAssertHandler = [](){ GTEST_FAIL(); };
	GlobalFatalAssertHandler = [](){ GTEST_FAIL(); };
#endif // DEBUG
}

inline void DisableFailOnAssert() noexcept
{
#ifdef DEBUG
	GlobalAssertHandler = [](){};
	GlobalFatalAssertHandler = [](){};
#endif // DEBUG
}
