#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

#include "../src/Rotator.h"

// dummy for debugging methods
#if (!defined DEBUG) && (!defined RELEASE)
	#define RELEASE
	#define WARN(message)
	#define WARN_IF(condition, message)
#endif

class Vector2D
{
public:
	Vector2D(float x, float y);
	Vector2D(const Vector2D& vector);
	Vector2D(Rotator rotator);
	virtual ~Vector2D(void);

	float x, y;

	/** Get vector length in units */
	float size(void);

	/** Get quarter of vector length (faster than Size()) */
	float qSize(void);

	/** Normalize vector */
	Vector2D ort(void);
	/** Mirror horisontally */
	Vector2D mirrorH();
	/** Mirror vertically */
	Vector2D mirrorV();
	/** Get normal-vector */
	Vector2D normal();

	/** Project this vector to line that parallel with the vector "base" */
	Vector2D project(Vector2D base);

	/** Get angle between vector and OX axis */
	Rotator rotation(void);

	friend Vector2D operator-(const Vector2D& vector);

	friend bool operator==(const Vector2D& left, const Vector2D& right);

	friend bool operator!=(const Vector2D& left, const Vector2D& right);

	friend Vector2D operator+(const Vector2D& left, const Vector2D& right);

    friend Vector2D operator+=(Vector2D& left, const Vector2D& right);

    friend Vector2D operator-(const Vector2D& left, const Vector2D& right);

    friend Vector2D operator-=(Vector2D& left, const Vector2D& right);

	friend Vector2D operator*(const Vector2D& vector, float scalar);
	friend Vector2D operator*(float scalar, const Vector2D& vector);

    friend Vector2D operator*=(Vector2D& vector, float scalar);
	friend Vector2D operator*=(float scalar, Vector2D& vector);

	friend Vector2D operator/(const Vector2D& vector, float scalar);

    friend Vector2D operator/=(Vector2D& vector, float scalar);

	friend float DotProduct(const Vector2D& left, const Vector2D& right);
};

extern const Vector2D LEFT_DIRECTION;
extern const Vector2D RIGHT_DIRECTION;
extern const Vector2D UP_DIRECTION;
extern const Vector2D DOWN_DIRECTION;
extern const Vector2D ZERO_VECTOR;

#endif
