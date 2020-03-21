#include "Base/precomp.h"

#include "GameData/Core/BoundingBox.h"


Vector2D BoundingBox::getFirst()
{
	return Vector2D(minX, minY);
}

Vector2D BoundingBox::getThird()
{
	return Vector2D(maxX, maxY);
}

Vector2D BoundingBox::getSecond()
{
	return Vector2D(maxX, minY);
}

Vector2D BoundingBox::getFourth()
{
	return Vector2D(minX, maxY);
}

BoundingBox operator+(const BoundingBox& left, const Vector2D& right)
{
	return BoundingBox(left.minX + right.x, left.minY + right.y, left.maxX + right.x, left.maxY + right.y);
}

static_assert(std::is_trivially_copyable<BoundingBox>(), "BoundingBox should be trivially copyable");
static_assert(std::is_trivially_destructible<BoundingBox>(), "BoundingBox should be trivially destructable");
