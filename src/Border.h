#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "../src/Vector2D.h"

class Border
{
public:
	Border(Vector2D a, Vector2D b);
	~Border(void);
	/** Get normal-vector for this border */
	Vector2D getNormal();
	/** Get first point of this segment */
	Vector2D getA();
	/** Get second point of this segment */
	Vector2D getB();
	/** Change first point of this segment (and recalculate normal) */
	void setA(Vector2D a);
	/** Change second point of this segment (and recalculate normal) */
	void setB(Vector2D b);
protected:
	/** First point */
	Vector2D a;
	/** Second point */
	Vector2D b;
	/** Normal of this border */
	Vector2D normal;
	/** Calculate normal for this hull */
	void calculateNormal();
};

#endif