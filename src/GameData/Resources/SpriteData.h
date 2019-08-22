#pragma once

#include <string>

#include "GameData/Core/ResourceHandle.h"
#include "GameData/Core/Vector2D.h"

class SpriteData
{
public:
	Vector2D mSize = ZERO_VECTOR;
	Vector2D mAnchor = Vector2D(0.5f, 0.5f);
	ResourceHandle mSpriteHandle;
};
