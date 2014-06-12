#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <stdio.h>

#include "../Core/Vector2D.h"
#include "../Core/Rotator.h"
#include "../Core/IActor.h"
#include "../Core/World.h"
#include "../Structures/BoundingBox.h"

namespace RayTrace
{
	bool fastTrace(World * world, const Vector2D &startPoint, const Vector2D &endPoint);
	IActor* trace(World * world, const Vector2D &startPoint, const Vector2D &endPoint,
		Vector2D *outPoint = nullptr, Vector2D *outNormal = nullptr);
};

#endif
