#include "Base/precomp.h"

#include <gtest/gtest.h>

#include "Utils/Geometry/Collide.h"

TEST(CollisionUtils, AreLinesParallel)
{
	EXPECT_TRUE(Collide::AreLinesParallel({10.0f, 20.0f}, {-10.0f, 20.0f}, {5.0f, -3.0f}, {-5.0f, -3.0f}));
	EXPECT_FALSE(Collide::AreLinesParallel({10.0f, 20.0f}, {-10.0f, 20.0f}, {5.0f, -3.0f}, {-5.0f, 3.0f}));
	EXPECT_TRUE(Collide::AreLinesParallel({20.0f, 10.0f}, {20.0f, -10.0f}, {-3.0f, 5.0f}, {-3.0f, -5.0f}));
	EXPECT_FALSE(Collide::AreLinesParallel({20.0f, 10.0f}, {20.0f, -10.0f}, {-3.0f, 5.0f}, {3.0f, -5.0f}));
	EXPECT_TRUE(Collide::AreLinesParallel({-10.0f, -10.0f}, {10.0f, 20.0f}, {5.0f, 0.0f}, {25.0f, 30.0f}));
	EXPECT_TRUE(Collide::AreLinesParallel({-10.0f, -10.0f}, {10.0f, 20.0f}, {25.0f, 30.0f}, {5.0f, 0.0f}));
	EXPECT_FALSE(Collide::AreLinesParallel({-10.0f, -10.0f}, {10.0f, 20.0f}, {25.0f, -30.0f}, {5.0f, 0.0f}));
}
