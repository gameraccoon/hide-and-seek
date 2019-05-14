#pragma once

#include <stdio.h>

#include "Core/Vector2D.h"
#include "Core/Rotator.h"
#include "Structures/BoundingBox.h"

class World;

namespace RayTrace
{
	bool FastTrace(World * world, const Vector2D &startPoint, const Vector2D &endPoint);
//	IActor* trace(World * world, const Vector2D &startPoint, const Vector2D &endPoint,
//		Vector2D *outPoint = nullptr, Vector2D *outNormal = nullptr);
	Vector2D GetPointIntersect2Lines(const Vector2D &A1, const Vector2D &A2, const Vector2D &B1, const Vector2D &B2);
}
