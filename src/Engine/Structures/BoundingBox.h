#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../Core/Vector2D.h"
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
	const Vector2D getFirst() const;
	/** Right-top point */
	const Vector2D getSecond() const;
	/** Right-bottom point */
	const Vector2D getThird() const;
	/** Left-bottom point */
	const Vector2D getFourth() const;
	/** X-pos of left border */
	float minX;
	/** Y-pos of top border */
	float minY;
	/** X-pos of right border */
	float maxX;
	/** Y-pos of bottom border */
	float maxY;
};

#endif
