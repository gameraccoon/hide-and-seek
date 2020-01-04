#include "GameData/Core/Rotator.h"

#include <nlohmann/json.hpp>

Rotator::Rotator(float angle)
	: mValue(angle)
{
	normalize();
}

float Rotator::getValue() const
{
	return mValue;
}

void Rotator::normalize()
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
	left.normalize();

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
	left.normalize();

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

static_assert(std::is_pod<Rotator>(), "Rotator should be pod type");
static_assert(std::is_trivially_copyable<Rotator>(), "Rotator should be trivially copyable");
