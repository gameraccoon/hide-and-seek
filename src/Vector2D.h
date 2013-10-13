#ifndef VECTOR_H
#define VECTOR_H

#include "../src/BaseProperties.h"

class Vector2D
{
public:
	Vector2D(float x, float y);
	Vector2D(const Vector2D& vect);
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

#endif