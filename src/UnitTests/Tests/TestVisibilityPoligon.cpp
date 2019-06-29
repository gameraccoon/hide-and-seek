#include <gtest/gtest.h>

#include <algorithm>

#include "GameData/Components/CollisionComponent.generated.h"
#include "GameData/Components/TransformComponent.generated.h"
#include "GameData/Core/Hull.h"
#include "Utils/Geometry/VisibilityPolygon.h"


static bool AreVisibilityPolygonsEqual(const std::vector<Vector2D>& a, const std::vector<Vector2D>& b)
{
	if (a.empty() && b.empty())
	{
		return true;
	}

	if (b.empty() || a.empty())
	{
		return false;
	}

	auto a_copy = a;

	auto it = std::find(a_copy.begin(), a_copy.end(), b.front());
	if (it == a_copy.end())
	{
		return false;
	}

	std::rotate(a_copy.begin(), it, a_copy.end());
	return a_copy == b;
}

TEST(VisibilityPolygon, TestPlygonsEqualityTest)
{
	EXPECT_TRUE(AreVisibilityPolygonsEqual({{30.0f, 30.0f}, {-50.0f, 50.0f}, {-10.0f, -10.0f}}, {{30.0f, 30.0f}, {-50.0f, 50.0f}, {-10.0f, -10.0f}}));
	EXPECT_TRUE(AreVisibilityPolygonsEqual({{30.0f, 30.0f}, {-50.0f, 50.0f}, {-10.0f, -10.0f}}, {{-50.0f, 50.0f}, {-10.0f, -10.0f}, {30.0f, 30.0f}}));
	EXPECT_TRUE(AreVisibilityPolygonsEqual({{30.0f, 30.0f}, {-50.0f, 50.0f}, {-10.0f, -10.0f}}, {{-10.0f, -10.0f}, {30.0f, 30.0f}, {-50.0f, 50.0f}}));
	EXPECT_FALSE(AreVisibilityPolygonsEqual({{30.0f, 30.0f}, {-50.0f, 50.0f}, {-10.0f, -10.0f}}, {{-10.0f, -10.0f}, {-50.0f, 50.0f}, {30.0f, 30.0f}}));
	EXPECT_FALSE(AreVisibilityPolygonsEqual({{30.0f, 30.0f}, {-50.0f, 50.0f}, {-10.0f, -10.0f}}, {{30.0f, 30.0f}, {-50.0f, 50.0f}}));
	EXPECT_FALSE(AreVisibilityPolygonsEqual({{30.0f, 30.0f}, {-50.0f, 50.0f}}, {{30.0f, 30.0f}, {-50.0f, 50.0f}, {-10.0f, -10.0f}}));
}

TEST(VisibilityPolygon, Empty)
{
	VisibilityPolygonCalculator calc;
	std::vector<std::tuple<CollisionComponent*, TransformComponent*>> components;
	Vector2D pos(-10.0f, 20.0f);

	std::vector<Vector2D> result;
	calc.calculateVisibilityPolygon(result, components, pos, Vector2D(600.0f, 600.0f));
	EXPECT_TRUE(AreVisibilityPolygonsEqual(result, {{-300.0f, -300.0f}, {300.0f, -300.0f}, {300.0f, 300.0f}, {-300.0f, 300.0f}}));
}

TEST(VisibilityPolygon, OneBorder)
{
	VisibilityPolygonCalculator calc;
	Vector2D pos(-10.0f, -40.0f);

	std::vector<std::tuple<CollisionComponent*, TransformComponent*>> components;
	CollisionComponent collision;
	Hull geometry;
	geometry.type = Hull::Type::Angular;
	geometry.points = {{-60.0f, -50.0f}, {60.0f, -50.0f}, {60.0f, 50.0f}, {-60.0f, 50.0f}};
	collision.setGeometry(geometry);
	TransformComponent transform;
	transform.setLocation(Vector2D(-10.0f, -210.0f));
	transform.setRotation(Rotator(0.0f));
	components.push_back(std::tuple<CollisionComponent*, TransformComponent*>(&collision, &transform));

	std::vector<Vector2D> result;
	calc.calculateVisibilityPolygon(result, components, pos, Vector2D(600.0f, 600.0f));
	EXPECT_TRUE(AreVisibilityPolygonsEqual(result, {{-300, -300}, {-150,-300}, {-60, -120}, {60, -120}, {150,-300}, {300, -300}, {300, 300}, {-300, 300}}));
}

TEST(VisibilityPolygon, OneBorderEqualAngles)
{
	VisibilityPolygonCalculator calc;
	Vector2D pos(308.0f, -33.0f);

	std::vector<std::tuple<CollisionComponent*, TransformComponent*>> components;
	CollisionComponent collision;
	Hull geometry;
	geometry.type = Hull::Type::Angular;
	geometry.points = {{60.0f, -60.0f}, {60.0f, 60.0f}, {-60.0f, 60.0f}, {-60.0f, -60.0f}};
	collision.setGeometry(geometry);
	TransformComponent transform;
	transform.setLocation(Vector2D(260.0f, 39.0f));
	transform.setRotation(Rotator(0.0f));
	components.push_back(std::tuple<CollisionComponent*, TransformComponent*>(&collision, &transform));

	std::vector<Vector2D> result;
	calc.calculateVisibilityPolygon(result, components, pos, Vector2D(600.0f, 600.0f));
	EXPECT_TRUE(AreVisibilityPolygonsEqual(result, {{-300.0f, -300.0f}, {300.0f, -300.0f}, {300.0f, 300.0f}, {12.0f, 12.0f}, {-108.0f, 12.0f}, {-300.0f, 33.3333f}}));
}
