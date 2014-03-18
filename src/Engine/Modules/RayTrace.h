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
	void setThickness(float thickness);
	bool fastTrace();
	IActor* trace(Vector2D *outPoint = NULL, Vector2D *outNormal = NULL);
protected:
	int getDotCode(const BoundingBox *box, const Vector2D *dot);
	bool checkIntersectAABBLine(const BoundingBox* box, const Vector2D* first, const Vector2D* last);
	bool checkIntersectVertLineWithLine(Vector2D A1, Vector2D A2, float x, float minY, float maxY);
	bool checkIntersectHoryLineWithLine(Vector2D A1, Vector2D A2, float y, float minX, float maxX);
	bool checkIntersect2Lines(Vector2D A1, Vector2D A2, Vector2D B1, Vector2D B2);
	Vector2D getPointIntersect2Lines(Vector2D A1, Vector2D A2, Vector2D B1, Vector2D B2);
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
	/** */
	float thickness;
	// helpers constaints
	static const int LEFT_BIT;
	static const int RIGHT_BIT;
	static const int BOTTOM_BIT;
	static const int TOP_BIT;
};

#endif
