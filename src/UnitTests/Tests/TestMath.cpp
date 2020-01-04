#include <gtest/gtest.h>

#include "Utils/Math.h"

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

TEST(Math, IsFloatGreater)
{
	const float epsilon = 0.001f;

	EXPECT_FALSE(Math::IsGreaterWithEpsilon(0.0f, 0.01f, epsilon));
	EXPECT_TRUE(Math::IsGreaterWithEpsilon(0.02f, 0.01f, epsilon));
	EXPECT_FALSE(Math::IsGreaterWithEpsilon(0.0f, 0.0f, epsilon));

	EXPECT_FALSE(Math::IsGreaterWithEpsilon(100000.0f, 100000.01f, epsilon));
	EXPECT_TRUE(Math::IsGreaterWithEpsilon(100000.02f, 100000.01f, epsilon));
	EXPECT_FALSE(Math::IsGreaterWithEpsilon(100000.0f, 100000.0f, epsilon));
}

TEST(Math, IsFloatGreaterOrEqual)
{
	const float epsilon = 0.001f;

	EXPECT_FALSE(Math::IsGreaterOrEqualWithEpsilon(0.0f, 0.01f, epsilon));
	EXPECT_TRUE(Math::IsGreaterOrEqualWithEpsilon(0.02f, 0.01f, epsilon));
	EXPECT_TRUE(Math::IsGreaterOrEqualWithEpsilon(0.0f, 0.0f, epsilon));

	EXPECT_FALSE(Math::IsGreaterOrEqualWithEpsilon(100000.0f, 100000.01f, epsilon));
	EXPECT_TRUE(Math::IsGreaterOrEqualWithEpsilon(100000.02f, 100000.01f, epsilon));
	EXPECT_TRUE(Math::IsGreaterOrEqualWithEpsilon(100000.0f, 100000.0f, epsilon));
}

TEST(Math, IsFloatLess)
{
	const float epsilon = 0.001f;

	EXPECT_TRUE(Math::IsLessWithEpsilon(0.0f, 0.01f, epsilon));
	EXPECT_FALSE(Math::IsLessWithEpsilon(0.02f, 0.01f, epsilon));
	EXPECT_FALSE(Math::IsLessWithEpsilon(0.0f, 0.0f, epsilon));

	EXPECT_TRUE(Math::IsLessWithEpsilon(100000.0f, 100000.01f, epsilon));
	EXPECT_FALSE(Math::IsLessWithEpsilon(100000.02f, 100000.01f, epsilon));
	EXPECT_FALSE(Math::IsLessWithEpsilon(100000.0f, 100000.0f, epsilon));
}

TEST(Math, IsFloatLessOrEqual)
{
	const float epsilon = 0.001f;

	EXPECT_TRUE(Math::IsLessOrEqualWithEpsilon(0.0f, 0.01f, epsilon));
	EXPECT_FALSE(Math::IsLessOrEqualWithEpsilon(0.02f, 0.01f, epsilon));
	EXPECT_TRUE(Math::IsLessOrEqualWithEpsilon(0.0f, 0.0f, epsilon));

	EXPECT_TRUE(Math::IsLessOrEqualWithEpsilon(100000.0f, 100000.01f, epsilon));
	EXPECT_FALSE(Math::IsLessOrEqualWithEpsilon(100000.02f, 100000.01f, epsilon));
	EXPECT_TRUE(Math::IsLessOrEqualWithEpsilon(100000.0f, 100000.0f, epsilon));
}
