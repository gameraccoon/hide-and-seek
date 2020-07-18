#pragma once

#include <nlohmann/json_fwd.hpp>

// 3.1415...
inline constexpr float PI = 3.14159265358979323846f;
inline constexpr float TwoPI = PI * 2.0f;
inline constexpr float HalfPI = PI * 0.5f;

/**
 * Hiding details of implementation of rotation angle
 *
 * Interval (-pi,pi]
*/
class Rotator
{
public:
	using UnderlyingType = float;
	static constexpr float MinValue = -PI;
	static constexpr float MaxValue = PI;

public:
	// leaves inner data uninitialized
	explicit Rotator() = default;

	explicit constexpr Rotator(float angle) noexcept
		: mValue(angle)
	{
		normalize();
	}

	[[nodiscard]] bool isNearlyEqualTo(Rotator other) const noexcept;

	[[nodiscard]] constexpr float getValue() const noexcept { return mValue; }

	[[nodiscard]] bool operator==(Rotator other) const noexcept;
	[[nodiscard]] bool operator!=(Rotator other) const noexcept;
	[[nodiscard]] Rotator operator+(Rotator other) const noexcept;
	Rotator operator+=(Rotator other) noexcept;
	[[nodiscard]] Rotator operator-(Rotator right) const noexcept;
	Rotator operator-=(Rotator right) noexcept;

	friend void to_json(nlohmann::json& outJson, const Rotator& rotator);
	friend void from_json(const nlohmann::json& json, Rotator& outRotator);

private:
	constexpr void normalize() noexcept
	{
		while (mValue <= -PI)
		{
			mValue += 2 * PI;
		}
		while (mValue > PI)
		{
			mValue -= 2 * PI;
		}
	}

private:
	float mValue;
};
