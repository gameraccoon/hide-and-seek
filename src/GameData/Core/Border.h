#pragma once

#include "GameData/Core/Vector2D.h"

class Border
{
public:
	Border(Vector2D a, Vector2D b) noexcept;

	/** Get normal-vector for this border */
	[[nodiscard]] Vector2D getNormal() const { return normal; }
	/** Get first point of this segment */
	[[nodiscard]] Vector2D getA() const { return mPointA; }
	/** Get second point of this segment */
	[[nodiscard]] Vector2D getB() const { return mPointB; }
	/** Change first point of this segment (and recalculate normal) */
	void setA(Vector2D a) noexcept;
	/** Change second point of this segment (and recalculate normal) */
	void setB(Vector2D b) noexcept;

private:
	void calculateNormal() noexcept;

private:
	Vector2D mPointA;
	Vector2D mPointB;
	/** Normal of this border */
	Vector2D normal;
};
