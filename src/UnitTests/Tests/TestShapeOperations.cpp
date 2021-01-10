#include "Base/precomp.h"

#include <gtest/gtest.h>

#include "Utils/Geometry/Collide.h"
#include "Utils/Geometry/ShapeOperations.h"

static std::vector<SimpleBorder> GenerateShape(const std::vector<Vector2D>& points)
{
	std::vector<SimpleBorder> result;
	size_t pointsCount = points.size();
	result.reserve(pointsCount);
	FOR_EACH_BORDER(pointsCount,
	{
		result.emplace_back(points[i], points[j]);
	});
	return result;
}

static std::vector<SimpleBorder> GenerateSimpleShape(const std::vector<Vector2D>& points)
{
	std::vector<SimpleBorder> result;
	size_t pointsCount = points.size();
	result.reserve(pointsCount);
	FOR_EACH_BORDER(pointsCount,
	{
		result.emplace_back(points[i], points[j]);
	});
	return result;
}

static bool AreShapesEqual(const std::vector<SimpleBorder>& a, const std::vector<SimpleBorder>& b)
{
	if (a.size() != b.size())
	{
		return false;
	}

	auto a_copy = a;

	for (const SimpleBorder& border : b)
	{
		a_copy.erase(
			std::remove_if(
				a_copy.begin(),
				a_copy.end(),
				[&border](const SimpleBorder& borderB){
					return border.a.isNearlyEqualTo(borderB.a) && border.b.isNearlyEqualTo(borderB.b);
				}),
			a_copy.end()
		);
	}
	return a_copy.empty();
}

TEST(ShapeOperations, Union_TwoTriangles)
{
	std::vector<Vector2D> shape1{{0.0f, 100.0f}, {100.0f, 0.0f}, {100.0f, 100.0f}};
	std::vector<Vector2D> shape2{{40.0f, 40.0f}, {80.0f, 70.0f}, {30.0f, 80.0f}};
	std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
	std::vector<Vector2D> expectedResult{{100.0f, 100.0f}, {0.0f, 100.0f}, {33.3333f, 66.6667f}, {40.0f, 40.0f}, {51.4286f, 48.5714f}, {100.0f, 0.0f}};
	EXPECT_TRUE(AreShapesEqual(GenerateSimpleShape(expectedResult), resultingShape));
	EXPECT_FALSE(AreShapesEqual(GenerateSimpleShape(std::vector<Vector2D>{{100.0f, 0.0f}, {0.0f, 100.0f}, {100.0f, 100.0f}}), resultingShape));
}

TEST(ShapeOperations, Union_TwoAxisAlignedRects)
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{60.0f, -10.0f}, {60.0f, 10.0f}, {-60.0f, 10.0f}, {-60.0f, -10.0f}};
	std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
	std::vector<Vector2D> expectedResult{{10.0f, -60.0f}, {10.0f, -10.0f}, {60.0f, -10.0f}, {60.0f, 10.0f}, {10.0f, 10.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, 10.0f}, {-60.0f, 10.0f}, {-60.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};
	EXPECT_TRUE(AreShapesEqual(GenerateSimpleShape(expectedResult), resultingShape));
}

TEST(ShapeOperations, Union_TwoRectsWithOneSameBorder)
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{10.0f, -10.0f}, {10.0f, 110.0f}, {-10.0f, 110.0f}, {-10.0f, -10.0f}};
	std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
	std::vector<Vector2D> expectedResult{{10.0f, -60.0f}, {10.0f, 110.0f}, {-10.0f, 110.0f}, {-10.0f, -60.0f}};
	EXPECT_TRUE(AreShapesEqual(GenerateSimpleShape(expectedResult), resultingShape));
}

TEST(ShapeOperations, Union_TwoRectsWithOneOverlappingBorder)
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{30.0f, -10.0f}, {30.0f, 110.0f}, {-30.0f, 110.0f}, {-30.0f, -10.0f}};
	std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
	std::vector<Vector2D> expectedResult{{30.0f, -10.0f}, {30.0f, 110.0f}, {-30.0f, 110.0f}, {-30.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}, {10.0f, -60.0f}, {10.0f, -10.0f}};
	EXPECT_TRUE(AreShapesEqual(GenerateSimpleShape(expectedResult), resultingShape));
}

TEST(ShapeOperations, Union_TwoRectsWithOneOverlappingBorderOneDirection)
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, 20.0f}, {-10.0f, 20.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{30.0f, -60.0f}, {30.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, -60.0f}};
	std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
	std::vector<Vector2D> expectedResult{{30.0f, -60.0f}, {30.0f, 10.0f}, {10.0f, 10.0f}, {10.0f, 20.0f}, {-10.0f, 20.0f}, {-10.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, -60.0f}};
	EXPECT_TRUE(AreShapesEqual(GenerateSimpleShape(expectedResult), resultingShape));
}

TEST(ShapeOperations, Union_TwoRectsWithOneOverlappingBorderOneDirectionFullyInside)
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{30.0f, -60.0f}, {30.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, -60.0f}};
	std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
	std::vector<Vector2D> expectedResult{{30.0f, -60.0f}, {30.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, -60.0f}};
	EXPECT_TRUE(AreShapesEqual(GenerateSimpleShape(expectedResult), resultingShape));
}

TEST(ShapeOperations, Union_TwoRectsWithTwoOverlappingBorders)
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{10.0f, -10.0f}, {10.0f, 110.0f}, {-10.0f, 110.0f}, {-10.0f, -10.0f}};
	std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
	std::vector<Vector2D> expectedResult{{10.0f, -60.0f}, {10.0f, 110.0f}, {-10.0f, 110.0f}, {-10.0f, -60.0f}};
TEST(ShapeOperations, Union_TwoRectsIntersectionOnCorner)
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{-30.0f, 10.0f}, {10.0f, -30.0f}, {20.0f, -20.0f}, {-20.0f, 20.0f}};
	std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
	std::vector<Vector2D> expectedResult{{10.0f, -60.0f}, {10.0f, -30.0f}, {20.0f, -20.0f}, {10.0f, -10.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, 10.0f}, {-20.0f, 20.0f}, {-30.0f, 10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};
	EXPECT_TRUE(AreShapesEqual(GenerateSimpleShape(expectedResult), resultingShape));
}