#include "Border.h"


Border::Border(Vector2D a, Vector2D b) : A(a), B(b), Normal((b - a).GetNormal())
{
}

Border::~Border(void)
{
}

Vector2D Border::GetNormal()
{
	return Normal;
}

Vector2D Border::GetA()
{
	return A;
}

Vector2D Border::GetB()
{
	return B;
}

void Border::SetA(Vector2D a)
{
	A = a;
	Normal = (B - a).GetNormal();
}

void Border::SetB(Vector2D b)
{
	B = b;
	Normal = (b - A).GetNormal();
}
