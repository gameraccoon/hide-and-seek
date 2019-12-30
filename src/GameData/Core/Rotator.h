#pragma once

#include <nlohmann/json_fwd.hpp>

// 3.1415...
extern const float PI;

/**
 * Hiding details of implementation of rotation angle
 *
 * Interval (-pi,pi]
*/
class Rotator
{
public:
	// leaves inner data uninitialized
	explicit Rotator() = default;
	explicit Rotator(float angle);

	/** Get value of angle */
	float getValue() const;
	/** Get rotator that mirrored from X-axis */
	friend Rotator operator-(Rotator rot);

	friend bool operator==(const Rotator& left, const Rotator& right);

	friend bool operator!=(const Rotator& left, const Rotator& right);

	friend Rotator operator+(const Rotator& left, const Rotator& right);

    friend Rotator operator+=(Rotator& left, const Rotator& right);

    friend Rotator operator-(const Rotator& left, const Rotator& right);

    friend Rotator operator-=(Rotator& left, const Rotator& right);

	friend void to_json(nlohmann::json& outJson, const Rotator& rotator);
	friend void from_json(const nlohmann::json& json, Rotator& outRotator);

private:
	void normalize();

private:
	float mValue;
};
