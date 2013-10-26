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
protected:
	byte GetDotCode(const BoundingBox *box, const Vector2D *dot);
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