#include "GameLogic/SharedManagers/TimeData.h"

#include <math.h>

static float myRoundFunc(float toRound)
{
	using namespace std; // to compile on both msvc and clang/gcc
	return round(toRound - 0.5);
}

void TimeData::update(float deltaTime)
{
	dt = deltaTime;
	currentTimestamp += static_cast<GameplayTimestamp::TimeValueType>(myRoundFunc(deltaTime * 300.0f));
}
