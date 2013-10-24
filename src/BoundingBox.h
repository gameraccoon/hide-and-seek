#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../src/Vector2D.h"

class BoundingBox
{
public:
	BoundingBox(float minX, float minY, float maxX, float maxY);
	BoundingBox(Vector2D minPoint, Vector2D maxPoint);
	~BoundingBox(void);
	Vector2D GetFirst();
	Vector2D GetSecond();
	Vector2D GetThird();
	Vector2D GetFourth();
	float MinX, MinY, MaxX, MaxY;
};

#endif