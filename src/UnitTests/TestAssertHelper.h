#pragma once

#include <gtest/gtest.h>

inline void EnableFailOnAssert() noexcept
{
#ifdef DEBUG_CHECKS
	GlobalAssertHandler = [](){ GTEST_FAIL(); };
	GlobalFatalAssertHandler = [](){ GTEST_FAIL(); };
	GlobalAllowAssertLogs = true;
#endif // DEBUG_CHECKS
}

inline void DisableFailOnAssert() noexcept
{
#ifdef DEBUG_CHECKS
	GlobalAssertHandler = [](){};
	GlobalFatalAssertHandler = [](){};
	GlobalAllowAssertLogs = false;
#endif // DEBUG_CHECKS
}
