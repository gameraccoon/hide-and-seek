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
	Vector2D(const Vector2D& vect);
	Vector2D(const Rotator& rot);
	virtual ~Vector2D(void);

	float X, Y;

	/** Get vector length in units */
	float Size(void);

	/** Get quarter of vector length (faster than Size()) */
	float QSize(void);

	/** Normalize vector */
	Vector2D Ort(void);

	/** Get angle between vector and OX axis */
	Rotator GetRotation(void);

	friend Vector2D operator-(const Vector2D& vect);

	friend bool operator==(const Vector2D& left, const Vector2D& right);

	friend bool operator!=(const Vector2D& left, const Vector2D& right);

	friend Vector2D operator+(const Vector2D& left, const Vector2D& right);

    friend Vector2D operator+=(Vector2D& left, const Vector2D& right);

    friend Vector2D operator-(const Vector2D& left, const Vector2D& right);

    friend Vector2D operator-=(Vector2D& left, const Vector2D& right);

	friend Vector2D operator*(const Vector2D& vect, float scalar);

    friend Vector2D operator*=(Vector2D& vect, float scalar);

	friend Vector2D operator/(const Vector2D& vect, float scalar);

    friend Vector2D operator/=(Vector2D& vect, float scalar);

	friend float DotProduct(const Vector2D& left, const Vector2D& right);

	/** Project vector "vect" on line that parallel with the vector "base" */
	friend Vector2D Project(Vector2D base, Vector2D vect);
};

extern const Vector2D LeftDirection;
extern const Vector2D RightDirection;
extern const Vector2D UpDirection;
extern const Vector2D DownDirection;
extern const Vector2D ZeroVector;

#endif
