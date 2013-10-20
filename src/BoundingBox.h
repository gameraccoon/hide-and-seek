#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../src/Vector2D.h"

class BoundingBox
{
public:
	BoundingBox(float minX, float minY, float maxX, float maxY);
	BoundingBox(Vector2D minPoint, Vector2D maxPoint);
	~BoundingBox(void);
	Vector2D GetMin();
	Vector2D GetMax();
	float MinX, MinY, MaxX, MaxY;
};

#endif