#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <stdio.h>

#include "../Core/Vector2D.h"
#include "../Core/Rotator.h"
#include "../Core/IActor.h"
#include "../Core/World.h"
#include "../Structures/BoundingBox.h"

class RayTrace
{
public:
	RayTrace(World * world, Vector2D start, Vector2D end);
	RayTrace(World * world, Vector2D start, Rotator rotation, float length);
	~RayTrace(void);
	bool fastTrace();
	IActor* trace(Vector2D *outPoint = nullptr, Vector2D *outNormal = nullptr);
protected:
	/** World where placed this helper */
	World *ownerWorld;
	/** Start point of the ray */
	Vector2D startPoint;
	/** End point of the ray (ray can't be infinite in this discrete system) */
	Vector2D endPoint;
	/** Angle between this ray and world's X-axis */
	Rotator angle;
	/** Length of ray (ray can't be infinite in this discrete system) */
	float rayLength;
};

#endif
