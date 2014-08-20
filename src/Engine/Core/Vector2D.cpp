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

Vector2D::Vector2D(const Rotator& rotator)
{
	float rotation = rotator.getValue();
	this->x = cos(rotation);
	this->y = sin(rotation);
}

Vector2D::~Vector2D(void)
{
}

float Vector2D::size(void) const
{
	return sqrt((this->x * this->x) + (this->y * this->y));
}

float Vector2D::qSize(void) const
{
	return (this->x * this->x) + (this->y * this->y);
}

const Vector2D Vector2D::ort(void) const
{
	if (this->x == 0.0f && this->y == 0.0f)
		return ZERO_VECTOR;

	return (*this) / this->size();
}

const Rotator Vector2D::rotation(void) const
{
	return atan2(this->y, this->x);
}

const Vector2D Vector2D::mirrorH() const
{
	return Vector2D(-this->x, this->y);
}

const Vector2D Vector2D::mirrorV() const
{
	return Vector2D(this->x, -this->y);
}

const Vector2D Vector2D::normal() const
{
	return this->rotation() - PI/2;
}

const Vector2D Vector2D::project(Vector2D base) const
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

Vector2D operator+=(Vector2D& left, const Vector2D& right)
{
	left.x += right.x;
	left.y += right.y;

	return Vector2D(left);
}

Vector2D operator+(const Vector2D& left, const Vector2D& right)
{
	return Vector2D(left) += right;
}

Vector2D operator-=(Vector2D& left, const Vector2D& right)
{
	left.x -= right.x;
	left.y -= right.y;

	return Vector2D(left);
}

Vector2D operator-(const Vector2D& left, const Vector2D& right)
{
	return Vector2D(left) -= right;
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

Vector2D operator*(const Vector2D& vector, float scalar)
{
	return Vector2D(vector) *= scalar;
}

Vector2D operator*(float scalar, const Vector2D& vector)
{
	return Vector2D(vector) *= scalar;
}

Vector2D operator/=(Vector2D& vector, float scalar)
{
	vector.x /= scalar;
	vector.y /= scalar;

	return Vector2D(vector);
}

Vector2D operator/(const Vector2D& vector, float scalar)
{
	return Vector2D(vector) /= scalar;
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
