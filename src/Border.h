#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "../src/Vector2D.h"

class Border
{
public:
	Border(Vector2D a, Vector2D b);
	~Border(void);
	/** Get normal-vector for this border */
	Vector2D GetNormal();
	/** Get first point of this segment */
	Vector2D GetA();
	/** Get second point of this segment */
	Vector2D GetB();
	/** Change first point of this segment (and recalculate normal) */
	void SetA(Vector2D a);
	/** Change second point of this segment (and recalculate normal) */
	void SetB(Vector2D b);
protected:
	/** First point */
	Vector2D A;
	/** Second point */
	Vector2D B;
	/** Normal of this border */
	Vector2D Normal;
};

#endif