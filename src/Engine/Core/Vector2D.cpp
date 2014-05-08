#include "Vector2D.h"

const float VECTOR_ERROR = 0.0001f;

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D::Vector2D(const Vector2D& vector)
{
	this->x = vector.x;
	this->y = vector.y;
}

Vector2D::Vector2D(Rotator rotator)
{
	this->x = cos(rotator.getValue());
	this->y = sin(rotator.getValue());
}

Vector2D::~Vector2D(void)
{
}

float Vector2D::size(void)
{
	return sqrt((this->x * this->x) + (this->y * this->y));
}

float Vector2D::qSize(void)
{
	return (this->x * this->x) + (this->y * this->y);
}

Vector2D Vector2D::ort(void)
{
	if (this->x == 0.0f && this->y == 0.0f)
		return ZERO_VECTOR;

	return (*this) / this->size();
}

Rotator Vector2D::rotation(void)
{
	return atan2(this->y, this->x);
}

Vector2D Vector2D::mirrorH()
{
	return Vector2D(-this->x, this->y);
}

Vector2D Vector2D::mirrorV()
{
	return Vector2D(this->x, -this->y);
}

Vector2D Vector2D::normal()
{
	return this->rotation() - PI/2;
}

Vector2D Vector2D::project(Vector2D base)
{
	float qSize = base.qSize();
	Vector2D result(ZERO_VECTOR);

	if (qSize != 0.0f)
	{
		float dProduct = DotProduct(base, (*this));
		result = base * dProduct / qSize;
	}

	return result;
}

Vector2D operator-(const Vector2D& vector)
{
	Vector2D newVect(vector);
	newVect.x = -vector.x;
	newVect.y = -vector.y;
	return newVect;
}

bool operator==(const Vector2D& left, const Vector2D& right)
{
	float vError = 0.0f;
	vError += abs(left.x - right.x);
	vError += abs(left.y - right.y);

	return vError <= VECTOR_ERROR;
}

bool operator!=(const Vector2D& left, const Vector2D& right)
{
	return !(left == right);
}

Vector2D operator+(const Vector2D& left, const Vector2D& right)
{
	Vector2D newVect(left);
	
	newVect.x += right.x;
	newVect.y += right.y;

	return newVect;
}

Vector2D operator+=(Vector2D& left, const Vector2D& right)
{
	left.x += right.x;
	left.y += right.y;

	return Vector2D(left);
}

Vector2D operator-(const Vector2D& left, const Vector2D& right)
{
	Vector2D newVect(left);
	
	newVect.x -= right.x;
	newVect.y -= right.y;

	return newVect;
}

Vector2D operator-=(Vector2D& left, const Vector2D& right)
{
	left.x -= right.x;
	left.y -= right.y;

	return Vector2D(left);
}

Vector2D operator*(const Vector2D& vect, float scalar)
{
	Vector2D newVect(vect);
	
	newVect.x *= scalar;
	newVect.y *= scalar;

	return newVect;
}

Vector2D operator*(float scalar, const Vector2D& vector)
{
	Vector2D newVect(vector);
	
	newVect.x *= scalar;
	newVect.y *= scalar;

	return newVect;
}

Vector2D operator*=(Vector2D& vector, float scalar)
{
	vector.x *= scalar;
	vector.y *= scalar;

	return Vector2D(vector);
}

Vector2D operator*=(float scalar, Vector2D& vector)
{
	vector.x *= scalar;
	vector.y *= scalar;

	return Vector2D(vector);
}

Vector2D operator/(const Vector2D& vector, float scalar)
{
	Vector2D newVect(vector);
	
	newVect.x /= scalar;
	newVect.y /= scalar;

	return newVect;
}

Vector2D operator/=(Vector2D& vector, float scalar)
{
	vector.x /= scalar;
	vector.y /= scalar;

	return Vector2D(vector);
}

float DotProduct(const Vector2D& left, const Vector2D& right)
{
	return left.x * right.x + left.y * right.y;
}

const Vector2D LEFT_DIRECTION(-1.0f, 0.0f);
const Vector2D RIGHT_DIRECTION(1.0f, 0.0f);
const Vector2D UP_DIRECTION(0.0f, -1.0f);
const Vector2D DOWN_DIRECTION(0.0f, 1.0f);
const Vector2D ZERO_VECTOR(0.0f, 0.0f);
