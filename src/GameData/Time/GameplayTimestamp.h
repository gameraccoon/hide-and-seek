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

	bool isInitialized() const;

	bool operator==(GameplayTimestamp other) const;
	bool operator!=(GameplayTimestamp other) const;
	bool operator<(GameplayTimestamp other) const;
	bool operator<=(GameplayTimestamp other) const;
	bool operator>(GameplayTimestamp other) const;
	bool operator>=(GameplayTimestamp other) const;

	void increaseByFloatTime(float passedTime);
	GameplayTimestamp getIncreasedByFloatTime(float passedTime) const;

	friend void to_json(nlohmann::json& outJson, const GameplayTimestamp timestamp);
	friend void from_json(const nlohmann::json& json, GameplayTimestamp& outTimestamp);

public:
	static constexpr float TimeMultiplier = 300.0f;

private:
	static const TimeValueType UNINITIALIZED_TIME = std::numeric_limits<TimeValueType>::max();
	TimeValueType mTimestamp = UNINITIALIZED_TIME;
};
