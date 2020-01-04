#pragma once

class CollisionComponent;
struct Vector2D;

namespace Collide
{
	float SignedArea(const Vector2D& a, const Vector2D& b, const Vector2D& c);

	bool DoCollide(const CollisionComponent* collisionA, const Vector2D& locationA,
				   const CollisionComponent* collisionB, const Vector2D& locationB, Vector2D& outResist);

	void UpdateBoundingBox(CollisionComponent* collision);
}
