#include "Border.h"


Border::Border(Vector2D a, Vector2D b) : a(a),
										b(b),
										normal((b - a).normal())
{
}

Border::~Border(void)
{
}

Vector2D Border::getNormal()
{
	return this->normal;
}

Vector2D Border::getA()
{
	return this->a;
}

Vector2D Border::getB()
{
	return this->b;
}

void Border::setA(Vector2D a)
{
	this->a = a;
	calculateNormal();
}

void Border::setB(Vector2D b)
{
	this->b = b;
	calculateNormal();
}

void Border::calculateNormal()
{
	this->normal = (this->b - this->a).normal();

}
