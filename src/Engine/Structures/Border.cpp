#include "Border.h"


Border::Border(const Vector2D& a, const Vector2D& b) : aPoint(a),
										bPoint(b),
										normal((b - a).normal())
{
}

Border::~Border()
{
}

Vector2D Border::getNormal() const
{
	return normal;
}

Vector2D Border::getA() const
{
	return aPoint;
}

Vector2D Border::getB() const
{
	return bPoint;
}

void Border::setA(const Vector2D& a)
{
	aPoint = a;
	calculateNormal();
}

void Border::setB(const Vector2D& b)
{
	bPoint = b;
	calculateNormal();
}

void Border::calculateNormal()
{
	normal = (bPoint - aPoint).normal();
}
