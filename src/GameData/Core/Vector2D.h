#pragma once

#include <math.h>

#include "Rotator.h"

#include <nlohmann/json_fwd.hpp>

struct Vector2D
{
	float x;
	float y;

	// leaves inner data uninitialized
	Vector2D() = default;
	constexpr Vector2D(float x, float y) : x(x), y(y) {}
	explicit Vector2D(const Rotator& rotator);

	/** Get vector length in units */
	float size() const;

	/** Get quarter of vector length (faster than Size()) */
	float qSize() const;

	bool isZeroLength() const;

	/** Normalize vector */
	Vector2D unit() const;
	/** Mirror horisontally */
	Vector2D mirrorH() const;
	/** Mirror vertically */
	Vector2D mirrorV() const;
	/** Get normal-vector */
	Vector2D normal() const;

	/** Project this vector to line that parallel with the vector "base" */
	Vector2D project(const Vector2D& base) const;

	/** Check that current point is inside an AABB rect with the given top-left and bottom-right points */
	bool isInside(const Vector2D& lt, const Vector2D& rb);

	/** Get angle between vector and OX axis */
	Rotator rotation() const;

	friend bool operator==(const Vector2D& left, const Vector2D& right);
	friend bool operator!=(const Vector2D& left, const Vector2D& right);

	friend Vector2D operator-(const Vector2D& vector);

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
	friend Vector2D HadamardProduct(const Vector2D& left, const Vector2D& right);

	friend void to_json(nlohmann::json& outJson, const Vector2D& vector);
	friend void from_json(const nlohmann::json& json, Vector2D& outVector);
};

extern const Vector2D LEFT_DIRECTION;
extern const Vector2D RIGHT_DIRECTION;
extern const Vector2D UP_DIRECTION;
extern const Vector2D DOWN_DIRECTION;
extern const Vector2D ZERO_VECTOR;

namespace std
{
	template <>
	struct hash<Vector2D>
	{
		std::size_t operator()(const Vector2D& k) const
		{
			return hash<float>()(k.x) ^ (hash<float>()(k.y) << 1);
		}
	};
}
