#include "Rotator.h"

const float PI = 3.14159265358979323846f;

Rotator::Rotator(float angle)
{
	Value = angle;
}

Rotator::~Rotator()
{
}

float Rotator::GetValue()
{
	float normValue = Value;
	if (normValue <= -PI)
	{
		while (normValue <= -PI)
		{
			normValue += 2 * PI;
		}

		Value = normValue;
	}
	else if (normValue > PI)
	{
		while (normValue > PI)
		{
			normValue -= 2 * PI;
		}

		Value = normValue;
	}

	return normValue;
}

Rotator operator-(Rotator rot)
{
	return Rotator(-rot.GetValue());
}

bool operator==(const Rotator& left, const Rotator& right)
{
	return left.Value == right.Value;
}

bool operator!=(const Rotator& left, const Rotator& right)
{
	return !(left == right);
}

Rotator operator+(const Rotator& left, const Rotator& right)
{
	return left.Value + right.Value;
}

Rotator operator+=(Rotator& left, const Rotator& right)
{
	float newValue = left.Value + right.Value;

	left.Value = newValue;

	return newValue;
}

Rotator operator-(const Rotator& left, const Rotator& right)
{
	return left.Value - right.Value;
}

Rotator operator-=(Rotator& left, const Rotator& right)
{
	float newValue = left.Value - right.Value;

	left.Value = newValue;

	return newValue;
}