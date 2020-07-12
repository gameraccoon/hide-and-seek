#pragma once

#include <gtest/gtest.h>

inline void EnableFailOnAssert() noexcept
{
#ifdef DEBUG_CHECKS
	GlobalAssertHandler = [](){ GTEST_FAIL(); };
	GlobalFatalAssertHandler = [](){ GTEST_FAIL(); };
#endif // DEBUG_CHECKS
}

inline void DisableFailOnAssert() noexcept
{
#ifdef DEBUG_CHECKS
	GlobalAssertHandler = [](){};
	GlobalFatalAssertHandler = [](){};
#endif // DEBUG_CHECKS
}
