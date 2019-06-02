#pragma once

#include <vector>
#include <array>

#include <nlohmann/json_fwd.hpp>

#include <Core/Vector2D.h>
#include "Border.h"

class NavMesh
{
private:
public:
	std::vector<std::array<Vector2D, 3>> mTriangles;
};
