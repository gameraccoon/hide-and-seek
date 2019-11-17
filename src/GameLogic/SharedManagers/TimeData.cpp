#include "GameLogic/SharedManagers/TimeData.h"

#include <math.h>

void TimeData::update(float deltaTime)
{
	dt = deltaTime;
	currentTimestamp += static_cast<GameplayTimestamp::TimeValueType>(std::round(deltaTime * 300.0f));
}
