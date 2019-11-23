#include "GameData/Spatial/CellPos.h"

CellPos::CellPos(int x, int y)
	: x(x)
	, y(y)
{
}

bool CellPos::operator==(const CellPos& other) const
{
	return x == other.x && y == other.y;
}
