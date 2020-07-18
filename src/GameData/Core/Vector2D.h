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
	// can be created from initializer list
	constexpr Vector2D(float x, float y) : x(x), y(y) {}
	explicit Vector2D(Rotator rotator) noexcept;

	/** Get vector length in units */
	[[nodiscard]] float size() const noexcept;

	/** Get quarter of vector length (faster than Size()) */
	[[nodiscard]] float qSize() const noexcept;

	[[nodiscard]] bool isZeroLength() const noexcept;

	/** Normalize vector */
	[[nodiscard]] Vector2D unit() const noexcept;
	/** Mirror horisontally */
	[[nodiscard]] Vector2D mirrorH() const noexcept;
	/** Mirror vertically */
	[[nodiscard]] Vector2D mirrorV() const noexcept;
	/** Get normal-vector */
	[[nodiscard]] Vector2D normal() const noexcept;

	/** Project this vector to line that parallel with the vector "base" */
	[[nodiscard]] Vector2D project(Vector2D base) const noexcept;

	/** Check that current point is inside an AABB rect with the given top-left and bottom-right points */
	[[nodiscard]] bool isInsideRect(Vector2D lt, Vector2D rb) const noexcept;

	/** Get angle between vector and OX axis */
	[[nodiscard]] Rotator rotation() const noexcept;

	// check for exact equality
	[[nodiscard]] bool operator==(Vector2D other) const noexcept;
	[[nodiscard]] bool operator!=(Vector2D other) const noexcept;

	[[nodiscard]] bool isNearlyEqualTo(Vector2D other) const noexcept;

	[[nodiscard]] Vector2D operator-() const noexcept;

	[[nodiscard]] Vector2D operator+(Vector2D other) const noexcept;
	Vector2D operator+=(Vector2D other) noexcept;

	[[nodiscard]] Vector2D operator-(Vector2D right) const noexcept;
	Vector2D operator-=(Vector2D right) noexcept;

	[[nodiscard]] Vector2D operator*(float scalar) const noexcept;
	[[nodiscard]] friend Vector2D operator*(float scalar, Vector2D vector) noexcept;
	Vector2D operator*=(float scalar) noexcept;

	[[nodiscard]] Vector2D operator/(float scalar) const noexcept;
	Vector2D operator/=(float scalar) noexcept;

	[[nodiscard]] friend float DotProduct(const Vector2D& left, const Vector2D& right) noexcept;
	[[nodiscard]] friend Vector2D HadamardProduct(const Vector2D& left, const Vector2D& right) noexcept;

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
