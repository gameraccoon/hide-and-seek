#pragma once

#include <vector>
#include <array>

#include <nlohmann/json_fwd.hpp>

#include "GameData/Core/Vector2D.h"
#include "Border.h"

class dtNavMesh;

class NavMesh
{
public:
	~NavMesh();

	void setMesh(dtNavMesh* newMesh);
	dtNavMesh* getMesh();

private:
	dtNavMesh* mMesh = nullptr;
};
