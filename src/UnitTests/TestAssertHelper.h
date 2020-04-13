#pragma once

#include <gtest/gtest.h>

inline void EnableFailOnAssert() noexcept
{
	GlobalAssertHandler = [](){ GTEST_FAIL(); };
	GlobalFatalAssertHandler = [](){ GTEST_FAIL(); };
}

inline void DisableFailOnAssert() noexcept
{
	GlobalAssertHandler = [](){};
	GlobalFatalAssertHandler = [](){};
}
