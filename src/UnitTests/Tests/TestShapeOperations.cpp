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

template <typename T>
static std::vector<T> JoinVectors(std::vector<T>&& a, std::vector<T>&& b)
{
	a.reserve(a.size() + b.size());
	std::move(b.begin(), b.end(), std::back_inserter(a));
	return std::move(a);
}


static bool TestShapesUnionResultIsCorrect(const std::vector<Vector2D>& shape1, const std::vector<Vector2D>& shape2, const std::vector<SimpleBorder>& expectedResult)
{
	{
		std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
		ShapeOperations::OptimizeShape(resultingShape);
		if (!AreShapesEqual(expectedResult, resultingShape)) { return false; }
	}
	{
		std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape2), GenerateShape(shape1));
		ShapeOperations::OptimizeShape(resultingShape);
		if (!AreShapesEqual(expectedResult, resultingShape)) { return false; }
	}
	return true;
}

TEST(ShapeOperations, Union_TwoTriangles)
{
	std::vector<Vector2D> shape1{{0.0f, 100.0f}, {100.0f, 0.0f}, {100.0f, 100.0f}};
	std::vector<Vector2D> shape2{{40.0f, 40.0f}, {80.0f, 70.0f}, {30.0f, 80.0f}};
	std::vector<Vector2D> expectedResult{{100.0f, 100.0f}, {0.0f, 100.0f}, {33.3333f, 66.6667f}, {40.0f, 40.0f}, {51.4286f, 48.5714f}, {100.0f, 0.0f}};

	{
		std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape1), GenerateShape(shape2));
		ShapeOperations::OptimizeShape(resultingShape);
		EXPECT_TRUE(AreShapesEqual(GenerateShape(expectedResult), resultingShape));
		EXPECT_FALSE(AreShapesEqual(GenerateShape(std::vector<Vector2D>{{100.0f, 0.0f}, {0.0f, 100.0f}, {100.0f, 100.0f}}), resultingShape));
	}

	{
		std::vector<SimpleBorder> resultingShape = ShapeOperations::GetUnion(GenerateShape(shape2), GenerateShape(shape1));
		ShapeOperations::OptimizeShape(resultingShape);
		EXPECT_TRUE(AreShapesEqual(GenerateShape(expectedResult), resultingShape));
		EXPECT_FALSE(AreShapesEqual(GenerateShape(std::vector<Vector2D>{{100.0f, 0.0f}, {0.0f, 100.0f}, {100.0f, 100.0f}}), resultingShape));
	}
}

/*bool Union_TwoAxisAlignedRects()
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{60.0f, -10.0f}, {60.0f, 10.0f}, {-60.0f, 10.0f}, {-60.0f, -10.0f}};
	std::vector<Vector2D> expectedResult{{10.0f, -60.0f}, {10.0f, -10.0f}, {60.0f, -10.0f}, {60.0f, 10.0f}, {10.0f, 10.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, 10.0f}, {-60.0f, 10.0f}, {-60.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};

	return TestShapesUnionResultIsCorrect(shape1, shape2, GenerateShape(expectedResult));
}*/

bool Union_TwoRectsWithOneSameBorder()
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{10.0f, -10.0f}, {10.0f, 110.0f}, {-10.0f, 110.0f}, {-10.0f, -10.0f}};
	std::vector<Vector2D> expectedResult{{10.0f, -60.0f}, {10.0f, 110.0f}, {-10.0f, 110.0f}, {-10.0f, -60.0f}};

	return TestShapesUnionResultIsCorrect(shape1, shape2, GenerateShape(expectedResult));
}

bool Union_TwoRectsWithOneOverlappingBorder()
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{30.0f, -10.0f}, {30.0f, 110.0f}, {-30.0f, 110.0f}, {-30.0f, -10.0f}};
	std::vector<Vector2D> expectedResult{{30.0f, -10.0f}, {30.0f, 110.0f}, {-30.0f, 110.0f}, {-30.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}, {10.0f, -60.0f}, {10.0f, -10.0f}};

	return TestShapesUnionResultIsCorrect(shape1, shape2, GenerateShape(expectedResult));
}

bool Union_TwoRectsWithOneOverlappingBorderOneDirection()
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, 20.0f}, {-10.0f, 20.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{30.0f, -60.0f}, {30.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, -60.0f}};
	std::vector<Vector2D> expectedResult{{30.0f, -60.0f}, {30.0f, 10.0f}, {10.0f, 10.0f}, {10.0f, 20.0f}, {-10.0f, 20.0f}, {-10.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, -60.0f}};

	return TestShapesUnionResultIsCorrect(shape1, shape2, GenerateShape(expectedResult));
}

bool Union_TwoRectsWithOneOverlappingBorderOneDirectionFullyInside()
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, -10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{30.0f, -60.0f}, {30.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, -60.0f}};
	std::vector<Vector2D> expectedResult{{30.0f, -60.0f}, {30.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, -60.0f}};

	return TestShapesUnionResultIsCorrect(shape1, shape2, GenerateShape(expectedResult));
}

bool Union_TwoRectsWithTwoOverlappingBorders()
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{10.0f, -10.0f}, {10.0f, 110.0f}, {-10.0f, 110.0f}, {-10.0f, -10.0f}};
	std::vector<Vector2D> expectedResult{{10.0f, -60.0f}, {10.0f, 110.0f}, {-10.0f, 110.0f}, {-10.0f, -60.0f}};

	return TestShapesUnionResultIsCorrect(shape1, shape2, GenerateShape(expectedResult));
}

/*bool Union_TwoRectsIntersectionOnCorner()
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{-30.0f, 10.0f}, {10.0f, -30.0f}, {20.0f, -20.0f}, {-20.0f, 20.0f}};
	std::vector<Vector2D> expectedResult{{10.0f, -60.0f}, {10.0f, -30.0f}, {20.0f, -20.0f}, {10.0f, -10.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, 10.0f}, {-20.0f, 20.0f}, {-30.0f, 10.0f}, {-10.0f, -10.0f}, {-10.0f, -60.0f}};
	return TestShapesUnionResultIsCorrect(shape1, shape2, GenerateShape(expectedResult));
}*/

bool Union_TwoRectsTouchingCornerAndBorder()
{
	std::vector<Vector2D> shape1{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}};
	std::vector<Vector2D> shape2{{10.0f, 10.0f}, {50.0f, -30.0f}, {60.0f, -20.0f}, {20.0f, 20.0f}};
	std::vector<SimpleBorder> expectedShape = JoinVectors(GenerateShape(shape1), GenerateShape(shape2));

	return TestShapesUnionResultIsCorrect(shape1, shape2, expectedShape);
}


bool Union_NonConvexShapeOneOverlappingBorder()
{
	std::vector<Vector2D> shape1{{-20.0f, 20.0f}, {-20.0f, -20.0f}, {20.0f, -20.0f}, {20.0f, 20.0f}, {10.0f, 20.0f}, {10.0f, 0.0f}, {-10.0f, 0.0f}, {-10.0f, 20.0f}};
	std::vector<Vector2D> shape2{{-30.0f, 10.0f}, {-30.0f, 0.0f}, {30.0f, 0.0f}, {30.0f, 10.0f}};
	std::vector<Vector2D> expectedResult{{-20.0f, 20.0f}, {-20.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, 0.0f}, {-20.0f, 0.0f}, {-20.0f, -20.0f}, {20.0f, -20.0f}, {20.0f, 0.0f}, {30.0f, 0.0f}, {30.0f, 10.0f}, {20.0f, 10.0f}, {20.0f, 20.0f}, {10.0f, 20.0f}, {10.0f, 10.0f}, {-10.0f, 10.0f}, {-10.0f, 20.0f}};

	return TestShapesUnionResultIsCorrect(shape1, shape2, GenerateShape(expectedResult));
}

bool Union_NonConvexShapeOneOverlappingBorderOneDirection()
{
	std::vector<Vector2D> shape1{{-20.0f, 20.0f}, {-20.0f, -20.0f}, {20.0f, -20.0f}, {20.0f, 20.0f}, {10.0f, 20.0f}, {10.0f, 0.0f}, {-10.0f, 0.0f}, {-10.0f, 20.0f}};
	std::vector<Vector2D> shape2{{-30.0f, 0.0f}, {-30.0f, -10.0f}, {30.0f, -10.0f}, {30.0f, 0.0f}};
	std::vector<Vector2D> expectedResult{{-20.0f, 20.0f}, {-20.0f, 0.0f}, {-30.0f, 0.0f}, {-30.0f, -10.0f}, {-20.0f, -10.0f}, {-20.0f, -20.0f}, {20.0f, -20.0f}, {20.0f, -10.0f}, {30.0f, -10.0f}, {30.0f, 0.0f}, {20.0f, 0.0f}, {20.0f, 20.0f}, {10.0f, 20.0f}, {10.0f, 0.0f}, {-10.0f, 0.0f}, {-10.0f, 20.0f}};

	return TestShapesUnionResultIsCorrect(shape1, shape2, GenerateShape(expectedResult));
}

void Union_TestAll()
{
	using boolFnPtr = bool(*)();
	std::vector<boolFnPtr> testFns;
	testFns.push_back(&Union_TwoRectsWithOneSameBorder);
	testFns.push_back(&Union_TwoRectsWithOneOverlappingBorder);
	testFns.push_back(&Union_TwoRectsWithOneOverlappingBorderOneDirection);
	testFns.push_back(&Union_TwoRectsWithOneOverlappingBorderOneDirectionFullyInside);
	testFns.push_back(&Union_TwoRectsWithTwoOverlappingBorders);
	testFns.push_back(&Union_TwoRectsTouchingCornerAndBorder);
	testFns.push_back(&Union_NonConvexShapeOneOverlappingBorder);
	testFns.push_back(&Union_NonConvexShapeOneOverlappingBorderOneDirection);

	for (uint32_t i = 100000u; i != 0; ++i)
	{
		std::array<bool, 32> flags;
		for (size_t j = 0; j < 32; ++j)
		{
			flags[j] = (i >> j) & 0x1;
		}
		ShapeOperations::setFlags(flags);

		for (auto fn : testFns)
		{
			bool res = fn();
			if (!res)
			{
				goto bad_label;
			}
		}
		ShapeOperations::printFlags();

		bad_label:
		if (i % 100000 == 0) {
			LogInfo("log %u %f%%", i, (static_cast<float>(i) / static_cast<float>(std::numeric_limits<uint32_t>::max()) * 100.0f ));
		}
		continue;
	}
}

TEST(ShapeOperations, OptimizeShape_OneExtraPoint)
{
	std::vector<SimpleBorder> shape = GenerateShape(std::vector<Vector2D>{{10.0f, -60.0f}, {10.0f, -40.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}});
	std::vector<SimpleBorder> expectedShape = GenerateShape(std::vector<Vector2D>{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}});
	ShapeOperations::OptimizeShape(shape);
	EXPECT_TRUE(AreShapesEqual(expectedShape, shape));
}

TEST(ShapeOperations, OptimizeShape_TwoExtraPointOnABorder)
{
	std::vector<SimpleBorder> shape = GenerateShape(std::vector<Vector2D>{{10.0f, -60.0f}, {10.0f, -40.0f}, {10.0f, 40.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}});
	std::vector<SimpleBorder> expectedShape = GenerateShape(std::vector<Vector2D>{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}});
	ShapeOperations::OptimizeShape(shape);
	EXPECT_TRUE(AreShapesEqual(expectedShape, shape));
}

TEST(ShapeOperations, OptimizeShape_DuplicatedPoint)
{
	std::vector<SimpleBorder> shape = GenerateShape(std::vector<Vector2D>{{10.0f, -60.0f}, {10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}});
	std::vector<SimpleBorder> expectedShape = GenerateShape(std::vector<Vector2D>{{10.0f, -60.0f}, {10.0f, 60.0f}, {-10.0f, 60.0f}, {-10.0f, -60.0f}});
	ShapeOperations::OptimizeShape(shape);
	EXPECT_TRUE(AreShapesEqual(expectedShape, shape));
}

TEST(ShapeOperations, OptimizeShape_TwoFiguresTouchingWithMirroredAngle)
{
	std::vector<SimpleBorder> tempShape1 = GenerateShape(std::vector<Vector2D>{{0.0f, 0.0f}, {10.0f, 0.0f}, {10.0f, 10.0f}, {0.0f, 10.0f}});
	std::vector<SimpleBorder> tempShape2 = GenerateShape(std::vector<Vector2D>{{10.0f, 10.0f}, {20.0f, 10.0f}, {20.0f, 20.0f}, {10.0f, 20.0f}});
	std::vector<SimpleBorder> testShape = JoinVectors(std::move(tempShape1), std::move(tempShape2));
	// expect the shape to be unchanged, since we don't want borders with opposing directions to merge
	std::vector<SimpleBorder> expectedShape = testShape;
	ShapeOperations::OptimizeShape(testShape);
	EXPECT_TRUE(AreShapesEqual(expectedShape, testShape));
}

TEST(ShapeOperations, OptimizeShape_BorderSplitTwice)
{
	std::vector<SimpleBorder> testShape{
		{{-30.0f, 10.0f}, {-30.0f, -60.0f}},
		{{30.0f, 10.0f}, {-30.0f, 10.0f}},
		{{30.0f, -60.0f}, {30.0f, 10.0f}},
		{{-10.0f, -60.0f}, {10.0f, -60.0f}},
		{{-30.0f, -60.0f}, {-10.0f, -60.0f}},
		{{10.0f, -60.0f}, {30.0f, -60.0f}}
	};

	std::vector<SimpleBorder> expectedShape = GenerateShape(std::vector<Vector2D>{{30.0f, -60.0f}, {30.0f, 10.0f}, {-30.0f, 10.0f}, {-30.0f, -60.0f}});
	ShapeOperations::OptimizeShape(testShape);
	EXPECT_TRUE(AreShapesEqual(expectedShape, testShape));
}
