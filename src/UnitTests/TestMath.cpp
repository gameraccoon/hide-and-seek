#include <gtest/gtest.h>

#include "Modules/Math.h"

TEST(Math, AreFloatsEqual)
{
	const float epsilon = 0.001f;

	EXPECT_TRUE(Math::AreEqualWithEpsilon(0.0f, 0.0f, epsilon));
	EXPECT_TRUE(Math::AreEqualWithEpsilon(1.0f, 1.0f, epsilon));
	EXPECT_TRUE(Math::AreEqualWithEpsilon(100000.0f, 100000.0f, epsilon));
	EXPECT_TRUE(Math::AreEqualWithEpsilon(100000.0f, 100000.0000001f, epsilon));
	EXPECT_TRUE(Math::AreEqualWithEpsilon(0.01f, 0.01f, epsilon));
	EXPECT_FALSE(Math::AreEqualWithEpsilon(0.01f, 0.001f, epsilon));
	EXPECT_FALSE(Math::AreEqualWithEpsilon(1000.0f, 1000.1f, epsilon));
}
