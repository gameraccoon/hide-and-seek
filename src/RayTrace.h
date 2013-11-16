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
	bool FastTrace();
	//IActor* Trace(Vector2D out_point, Vector2D out_normal);
//protected:
	byte GetDotCode(const BoundingBox *box, const Vector2D *dot);
	bool CheckIntersectAABBLine(const BoundingBox* box, const Vector2D* first, const Vector2D* last);
	bool CheckIntersectVertLineWithLine(Vector2D A1, Vector2D A2, float x, float minY, float maxY);
	bool CheckIntersectHoryLineWithLine(Vector2D A1, Vector2D A2, float y, float minX, float maxX);
	bool CheckIntersect2Lines(Vector2D A1, Vector2D A2, Vector2D B1, Vector2D B2);
	bool GetPointIntersect2Lines(Vector2D A1, Vector2D A2, Vector2D B1, Vector2D B2, Vector2D *outIntersectionPoint = NULL);
	/** World where placed this helper */
	World *OwnerWorld;
	/** Start point of the ray */
	Vector2D StartPoint;
	/** End point of the ray (ray can't be infinite in this discrete system) */
	Vector2D EndPoint;
	/** Angle between this ray and world's X-axis */
	Rotator Angle;
	/** Length of ray (ray can't be infinite in this discrete system) */
	float RayLength;
	// helpers constaints
	static const byte LeftBit;
	static const byte RightBit;
	static const byte BottomBit;
	static const byte TopBit;
};

#endif