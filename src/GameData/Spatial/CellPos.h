#pragma once

#include <functional>

#include <nlohmann/json_fwd.hpp>

struct CellPos
{
	int x;
	int y;

	CellPos() = default;
	CellPos(int x, int y);
	bool operator==(const CellPos& other) const;
	bool operator!=(const CellPos& other) const;

	friend void to_json(nlohmann::json& outJson, const CellPos& pos);
	friend void from_json(const nlohmann::json& json, CellPos& outPos);
};

namespace std
{
	template<> struct hash<CellPos>
	{
		size_t operator()(const CellPos& cellPos) const noexcept
		{
			return hash<int>()(cellPos.x) ^ (hash<int>()(cellPos.y) << 1);
		}
	};
}
