#pragma once

class CollisionComponent;
struct Vector2D;

namespace Collide
{
	float SignedArea(const Vector2D& a, const Vector2D& b, const Vector2D& c);

	bool DoCollide(const CollisionComponent* collisionA, const Vector2D& locationA,
				   const CollisionComponent* collisionB, const Vector2D& locationB, Vector2D& outResist);

	void UpdateBoundingBox(CollisionComponent* collision);

	bool AreLinesIntersect(const Vector2D& A1, const Vector2D& A2, const Vector2D& B1, const Vector2D& B2);
	bool IsLineIntersectAABB(const BoundingBox& box, const Vector2D& start, const Vector2D& finish);
	Vector2D GetPointIntersect2Lines(const Vector2D& A1, const Vector2D& A2, const Vector2D& B1, const Vector2D& B2);
}
