#include "Base/precomp.h"

#include "GameData/Core/Border.h"


Border::Border(Vector2D a, Vector2D b) noexcept
	: mPointA(a)
	, mPointB(b)
	, normal((b - a).normal())
{
}

void Border::setA(Vector2D a) noexcept
{
	mPointA = a;
	calculateNormal();
}

void Border::setB(Vector2D b) noexcept
{
	mPointB = b;
	calculateNormal();
}

void Border::calculateNormal() noexcept
{
	normal = (mPointB - mPointA).normal();
}

static_assert(std::is_trivially_copyable<Border>(), "Border should be trivially copyable");
