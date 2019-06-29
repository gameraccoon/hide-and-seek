#include "GameData/Core/Vector2D.h"
#include <cmath>

#include <nlohmann/json.hpp>

static const float VECTOR_ERROR = 0.0001f;

const Vector2D LEFT_DIRECTION(-1.0f, 0.0f);
const Vector2D RIGHT_DIRECTION(1.0f, 0.0f);
const Vector2D UP_DIRECTION(0.0f, -1.0f);
const Vector2D DOWN_DIRECTION(0.0f, 1.0f);
const Vector2D ZERO_VECTOR(0.0f, 0.0f);

Vector2D::Vector2D(float x, float y)
	: x(x)
	, y(y)
{
}

Vector2D::Vector2D(const Rotator& rotator)
{
	float rotation = rotator.getValue();
	x = std::cos(rotation);
	y = std::sin(rotation);
}

float Vector2D::size() const
{
	return sqrt((x * x) + (y * y));
}

float Vector2D::qSize() const
{
	return (x * x) + (y * y);
}

Vector2D Vector2D::ort() const
{
	if (x == 0.0f && y == 0.0f)
		return ZERO_VECTOR;

	return (*this) / size();
}

Rotator Vector2D::rotation() const
{
	return Rotator(atan2(y, x));
}

Vector2D Vector2D::mirrorH() const
{
	return Vector2D(-x, y);
}

Vector2D Vector2D::mirrorV() const
{
	return Vector2D(x, -y);
}

Vector2D Vector2D::normal() const
{
	return Vector2D(rotation() - Rotator(PI * 0.5f));
}

Vector2D Vector2D::project(Vector2D base) const
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

void to_json(nlohmann::json& outJson, const Vector2D& vector)
{
	outJson = nlohmann::json{{"x", vector.x}, {"y", vector.y}};
}

void from_json(const nlohmann::json& json, Vector2D& outVector)
{
	json.at("x").get_to(outVector.x);
	json.at("y").get_to(outVector.y);
}

static_assert(sizeof(Vector2D) == sizeof(int)*2, "Vector2D is too big");
static_assert(std::is_pod<Vector2D>(), "Vector2D should be pod type");
static_assert(std::is_trivially_copyable<Vector2D>(), "Vector2D should be trivially copyable");
