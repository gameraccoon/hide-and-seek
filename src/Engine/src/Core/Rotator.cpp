#include "Core/Rotator.h"

#include <nlohmann/json.hpp>

const float PI = 3.14159265358979323846f;

Rotator::Rotator(float angle)
	: mValue(angle)
{
	reset();
}

float Rotator::getValue() const
{
	if (calculatedValue <= -PI)
	{
		while (calculatedValue <= -PI)
		{
			calculatedValue += 2 * PI;
		}
	}
	else if (calculatedValue > PI)
	{
		while (calculatedValue > PI)
		{
			calculatedValue -= 2 * PI;
		}
	}

	return calculatedValue;
}

void Rotator::reset()
{
	calculatedValue = mValue;
}

Rotator operator-(Rotator rot)
{
	return Rotator(-rot.getValue());
}

bool operator==(const Rotator& left, const Rotator& right)
{
	return left.getValue() == right.getValue();
}

bool operator!=(const Rotator& left, const Rotator& right)
{
	return !(left == right);
}

Rotator operator+(const Rotator& left, const Rotator& right)
{
	return Rotator(left.getValue() + right.getValue());
}

Rotator operator+=(Rotator& left, const Rotator& right)
{
	float newValue = left.getValue() + right.getValue();

	left.mValue = newValue;
	left.reset();

	return Rotator(newValue);
}

Rotator operator-(const Rotator& left, const Rotator& right)
{
	return Rotator(left.getValue() - right.getValue());
}

Rotator operator-=(Rotator& left, const Rotator& right)
{
	float newValue = left.getValue() - right.getValue();

	left.mValue = newValue;
	left.reset();

	return Rotator(newValue);
}

void to_json(nlohmann::json& outJson, const Rotator& rotator)
{
	outJson = nlohmann::json{{"v", rotator.mValue}};
}

void from_json(const nlohmann::json& json, Rotator& outRotator)
{
	json.at("v").get_to(outRotator.mValue);
}
