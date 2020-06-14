#include "Base/precomp.h"

#include "GameLogic/SharedManagers/TimeData.h"

#include <math.h>

void TimeData::update(float deltaTime)
{
	dt = deltaTime;
	currentTimestamp.increaseByFloatTime(dt);
}
