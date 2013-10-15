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

	/** Длина вектора в юнитах */
	float Size(void);

	/** Получить орт вектора (единичный вектор, направленный в ту же сторону что и данный) */
	Vector2D Ort(void);

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

	//friend Vector2D DotProfuct(const Vector2D& left, const Vector2D& right);
};

extern const Vector2D LeftDirection;
extern const Vector2D RightDirection;
extern const Vector2D UpDirection;
extern const Vector2D DownDirection;
extern const Vector2D ZeroVector;

#endif
