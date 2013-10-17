#include "Vector2D.h"

const float VECTOR_ERROR = 0.0001f;

Vector2D::Vector2D(float x, float y)
{
	X = x;
	Y = y;
}

Vector2D::Vector2D(const Vector2D& vect)
{
	X = vect.X;
	Y = vect.Y;
}

Vector2D::Vector2D(const Rotator& rot)
{
	X = cos(rot);
	Y = sin(rot);
}

Vector2D::~Vector2D(void)
{
}

float Vector2D::Size(void)
{
	return sqrt(X*X + Y*Y);
}

Vector2D Vector2D::Ort(void)
{
	if (X == 0.0f && Y == 0.0f)
		return ZeroVector;

	return *this / Size();
}

Rotator Vector2D::GetRotation(void)
{
	return atan2(Y, X);
}

Vector2D operator-(const Vector2D& vect)
{
	Vector2D newVect(vect);
	newVect.X = -vect.X;
	newVect.Y = -vect.Y;
	return newVect;
}

bool operator==(const Vector2D& left, const Vector2D& right)
{
	float vError = 0.0f;
	vError += abs(left.X - right.X);
	vError += abs(left.Y - right.Y);

	return vError <= VECTOR_ERROR;
}

bool operator!=(const Vector2D& left, const Vector2D& right)
{
	return !(left == right);
}

Vector2D operator+(const Vector2D& left, const Vector2D& right)
{
	Vector2D newVect(left);
	
	newVect.X += right.X;
	newVect.Y += right.Y;

	return newVect;
}

Vector2D operator+=(Vector2D& left, const Vector2D& right)
{
	left.X += right.X;
	left.Y += right.Y;

	return Vector2D(left);
}

Vector2D operator-(const Vector2D& left, const Vector2D& right)
{
	Vector2D newVect(left);
	
	newVect.X -= right.X;
	newVect.Y -= right.Y;

	return newVect;
}

Vector2D operator-=(Vector2D& left, const Vector2D& right)
{
	left.X -= right.X;
	left.Y -= right.Y;

	return Vector2D(left);
}

Vector2D operator*(const Vector2D& vect, float scalar)
{
	Vector2D newVect(vect);
	
	newVect.X *= scalar;
	newVect.Y *= scalar;

	return newVect;
}

Vector2D operator*=(Vector2D& vect, float scalar)
{
	vect.X *= scalar;
	vect.Y *= scalar;

	return Vector2D(vect);
}

Vector2D operator/(const Vector2D& vect, float scalar)
{
	Vector2D newVect(vect);
	
	newVect.X /= scalar;
	newVect.Y /= scalar;

	return newVect;
}

Vector2D operator/=(Vector2D& vect, float scalar)
{
	vect.X /= scalar;
	vect.Y /= scalar;

	return Vector2D(vect);
}

const Vector2D LeftDirection(-1.0f, 0.0f);
const Vector2D RightDirection(1.0f, 0.0f);
const Vector2D UpDirection(0.0f, -1.0f);
const Vector2D DownDirection(0.0f, 1.0f);
const Vector2D ZeroVector(0.0f, 0.0f);
