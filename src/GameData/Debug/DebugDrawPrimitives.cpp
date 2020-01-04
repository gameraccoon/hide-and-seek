#include "GameData/Time/GameplayTimestamp.h"

#include <type_traits>
#include <nlohmann/json.hpp>

bool GameplayTimestamp::isUnitialized() const
{
	return mTimestamp == UNINITIALIZED_TIME;
}

bool GameplayTimestamp::operator==(GameplayTimestamp other) const
{
	return mTimestamp == other.mTimestamp;
}

bool GameplayTimestamp::operator!=(GameplayTimestamp other) const
{
	return mTimestamp != other.mTimestamp;
}

bool GameplayTimestamp::operator<(GameplayTimestamp other) const
{
	return mTimestamp < other.mTimestamp;
}

bool GameplayTimestamp::operator<=(GameplayTimestamp other) const
{
	return mTimestamp <= other.mTimestamp;
}

bool GameplayTimestamp::operator>(GameplayTimestamp other) const
{
	return mTimestamp > other.mTimestamp;
}

bool GameplayTimestamp::operator>=(GameplayTimestamp other) const
{
	return mTimestamp >= other.mTimestamp;
}

GameplayTimestamp GameplayTimestamp::operator+(TimeValueType delta) const
{
	return GameplayTimestamp(mTimestamp + delta);
}

GameplayTimestamp GameplayTimestamp::operator+=(TimeValueType delta)
{
	return GameplayTimestamp(mTimestamp += delta);
}

GameplayTimestamp GameplayTimestamp::operator-(TimeValueType delta) const
{
	return GameplayTimestamp(mTimestamp - delta);
}

GameplayTimestamp GameplayTimestamp::operator-=(TimeValueType delta)
{
	return GameplayTimestamp(mTimestamp -= delta);
}

void to_json(nlohmann::json& outJson, const GameplayTimestamp timestamp)
{
	outJson = nlohmann::json::object({
		{"value", timestamp.mTimestamp}
	});
}

void from_json(const nlohmann::json& json, GameplayTimestamp& outTimestamp)
{
	json.at("value").get_to(outTimestamp.mTimestamp);
}

static_assert(std::is_trivially_copyable<GameplayTimestamp>(), "Vector2D should be trivially copyable");
