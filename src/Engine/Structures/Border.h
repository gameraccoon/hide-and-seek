#pragma once

#include "../Core/Vector2D.h"

class Border
{
public:
	Border(const Vector2D& a, const Vector2D& b);
	~Border();
	/** Get normal-vector for this border */
	Vector2D getNormal() const;
	/** Get first point of this segment */
	Vector2D getA() const;
	/** Get second point of this segment */
	Vector2D getB() const;
	/** Change first point of this segment (and recalculate normal) */
	void setA(const Vector2D& a);
	/** Change second point of this segment (and recalculate normal) */
	void setB(const Vector2D& b);
protected:
	/** First point */
	Vector2D aPoint;
	/** Second point */
	Vector2D bPoint;
	/** Normal of this border */
	Vector2D normal;
	/** Calculate normal for this hull */
	void calculateNormal();
};
