#pragma once

#include <nlohmann/json_fwd.hpp>
#include "Base/Types/BasicTypes.h"

class GameplayTimestamp
{
public:
	// as 1/30000 of a second
	using TimeValueType = u64;

public:
	GameplayTimestamp() = default;
	explicit GameplayTimestamp(TimeValueType time) : mTimestamp(time) {}

	bool isUnitialized() const;

	bool operator==(GameplayTimestamp other) const;
	bool operator!=(GameplayTimestamp other) const;
	bool operator<(GameplayTimestamp other) const;
	bool operator<=(GameplayTimestamp other) const;
	bool operator>(GameplayTimestamp other) const;
	bool operator>=(GameplayTimestamp other) const;

	GameplayTimestamp operator+(TimeValueType delta) const;
	GameplayTimestamp operator+=(TimeValueType delta);
	GameplayTimestamp operator-(TimeValueType delta) const;
	GameplayTimestamp operator-=(TimeValueType delta);

	friend void to_json(nlohmann::json& outJson, const GameplayTimestamp timestamp);
	friend void from_json(const nlohmann::json& json, GameplayTimestamp& outTimestamp);

private:
	static const TimeValueType UNINITIALIZED_TIME = 0;
	TimeValueType mTimestamp = UNINITIALIZED_TIME;
};
