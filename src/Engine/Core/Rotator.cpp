#include "Rotator.h"

const float PI = 3.14159265358979323846f;

Rotator::Rotator(float angle)
{
	this->value = angle;
}

Rotator::~Rotator()
{
}

float Rotator::getValue()
{
	float normValue = this->value;
	if (normValue <= -PI)
	{
		while (normValue <= -PI)
		{
			normValue += 2 * PI;
		}

		this->value = normValue;
	}
	else if (normValue > PI)
	{
		while (normValue > PI)
		{
			normValue -= 2 * PI;
		}

		this->value = normValue;
	}

	return normValue;
}

Rotator operator-(Rotator rot)
{
	return Rotator(-rot.getValue());
}

bool operator==(const Rotator& left, const Rotator& right)
{
	return left.value == right.value;
}

bool operator!=(const Rotator& left, const Rotator& right)
{
	return !(left == right);
}

Rotator operator+(const Rotator& left, const Rotator& right)
{
	return left.value + right.value;
}

Rotator operator+=(Rotator& left, const Rotator& right)
{
	float newValue = left.value + right.value;

	left.value = newValue;

	return newValue;
}

Rotator operator-(const Rotator& left, const Rotator& right)
{
	return left.value - right.value;
}

Rotator operator-=(Rotator& left, const Rotator& right)
{
	float newValue = left.value - right.value;

	left.value = newValue;

	return newValue;
}
