#include "Rotator.h"

const float PI = 3.14159265358979323846f;

Rotator::Rotator(float angle)
{
	this->value = angle;
	this->reset();
}

Rotator::~Rotator()
{
}

float Rotator::getValue() const
{
	if (this->calculatedValue <= -PI)
	{
		while (this->calculatedValue <= -PI)
		{
			this->calculatedValue += 2 * PI;
		}
	}
	else if (this->calculatedValue > PI)
	{
		while (this->calculatedValue > PI)
		{
			this->calculatedValue -= 2 * PI;
		}
	}

	return this->calculatedValue;
}

void Rotator::reset()
{
	this->calculatedValue = this->value;
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
	left.reset();

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
	left.reset();

	return newValue;
}