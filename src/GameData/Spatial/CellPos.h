#pragma once

#include <functional>

struct CellPos
{
	int x;
	int y;

	CellPos(int x, int y);
	bool operator==(const CellPos& other) const;
};

namespace std
{
	template<> struct hash<CellPos>
	{
		size_t operator()(const CellPos& cellPos) const noexcept
		{
			// it's already a unique hash
			return hash<int>()(cellPos.x) ^ (hash<int>()(cellPos.y) << 1);
		}
	};
}
