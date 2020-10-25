#pragma once

#include <vector>

#include "GameData/Core/Vector2D.h"
#include "GameData/Core/Border.h"

namespace ShapeOperations
{
	std::vector<SimpleBorder> GetUnion(const std::vector<SimpleBorder>& shape1, const std::vector<SimpleBorder>& shape2);
}
