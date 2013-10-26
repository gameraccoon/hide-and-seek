#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../src/Vector2D.h"
/**
 * Axis-aligned bounding box (AABB)
 */
class BoundingBox
{
public:
	BoundingBox(float minX, float minY, float maxX, float maxY);
	BoundingBox(Vector2D minPoint, Vector2D maxPoint);
	~BoundingBox(void);
	/** Left-top point */
	Vector2D GetFirst();
	/** Right-top point */
	Vector2D GetSecond();
	/** Right-bottom point */
	Vector2D GetThird();
	/** Left-bottom point */
	Vector2D GetFourth();
	/** X-pos of left border */
	float MinX;
	/** Y-pos of top border */
	float MinY;
	/** X-pos of right border */
	float MaxX;
	/** Y-pos of bottom border */
	float MaxY;
};

#endif