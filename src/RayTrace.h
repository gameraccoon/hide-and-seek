#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "../src/Vector2D.h"
#include "../src/Rotator.h"
#include "../src/IActor.h"
#include "../src/World.h"
#include "../src/BoundingBox.h"

class RayTrace
{
public:
	RayTrace(World * world, Vector2D start, Vector2D end);
	RayTrace(World * world, Vector2D start, Rotator rotation, float length);
	~RayTrace(void);
	bool fastTrace();
	IActor* trace(Vector2D *outPoint = NULL, Vector2D *outNormal = NULL);
protected:
	byte getDotCode(const BoundingBox *box, const Vector2D *dot);
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
	// helpers constaints
	static const byte LEFT_BIT;
	static const byte RIGHT_BIT;
	static const byte BOTTOM_BIT;
	static const byte TOP_BIT;
};

#endif
