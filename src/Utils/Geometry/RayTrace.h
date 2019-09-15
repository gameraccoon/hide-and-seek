#pragma once

class World;
class Vector2D;
class BoundingBox;

namespace RayTrace
{
	bool AreLinesIntersect(const Vector2D &A1, const Vector2D &A2, const Vector2D &B1, const Vector2D &B2);
	bool IsLineIntersectAABB(const BoundingBox &box, const Vector2D &first, const Vector2D &last);
	bool FastTrace(World * world, const Vector2D &startPoint, const Vector2D &endPoint);
//	IActor* trace(World * world, const Vector2D &startPoint, const Vector2D &endPoint,
//		Vector2D *outPoint = nullptr, Vector2D *outNormal = nullptr);
	Vector2D GetPointIntersect2Lines(const Vector2D &A1, const Vector2D &A2, const Vector2D &B1, const Vector2D &B2);
}
