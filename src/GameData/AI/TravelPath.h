#pragma once

#include <vector>

#include "GameData/Core/Vector2D.h"

class TravelPath
{
public:
	std::vector<Vector2D>& getSmoothPathRef() { return mSmoothPath; }
private:
	std::vector<Vector2D> mSmoothPath;
};
