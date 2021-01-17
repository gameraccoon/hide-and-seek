#pragma once

#include <vector>
#include <array>

#include "GameData/Core/Vector2D.h"
#include "GameData/Core/Border.h"

namespace ShapeOperations
{
	void setFlags(const std::array<bool, 32>& flags);
	void printFlags();

	std::vector<SimpleBorder> GetUnion(const std::vector<SimpleBorder>& shape1, const std::vector<SimpleBorder>& shape2);
	// join all borders that make a straight line
	void OptimizeShape(std::vector<SimpleBorder>& inOutShape);
}
