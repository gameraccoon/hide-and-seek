#include "Vector.h"


Vector::Vector(void)
{
	X = 0.0f;
	Y = 0.0f;
	Z = 0.0f;
}

Vector::Vector(const Vector& vect)
{
	X = vect.X;
	Y = vect.Y;
	Z = vect.Z;
}

Vector::~Vector(void)
{
}

const Vector operator+(const Vector& left, const Vector& right)
{
	Vector newVect(left);
	
	newVect.X += right.X;
	newVect.Y += right.Y;
	newVect.Z += right.Z;

	return newVect;
}

Vector& operator+=(Vector& left, const Vector& right)
{
	left.X += right.X;
	left.Y += right.Y;
	left.Z += right.Z;
	return Vector(left);
}

bool operator==(const Vector& left, const Vector& right)
{
	return left.X == right.X && left.Y == right.Y && left.Z == right.Z;
}