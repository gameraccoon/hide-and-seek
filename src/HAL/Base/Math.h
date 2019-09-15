#pragma once

#include "HAL/Base/Types.h"

#include "GameData/Core/Vector2D.h"
#include "Utils/Math.h"

namespace Graphics
{
	inline Vector2D QuadLerp(const QuadUV& uv, const Vector2D& v)
	{
		return Vector2D(std::lerp(uv.U1, uv.U2, v.x), std::lerp(uv.V1, uv.V2, v.y));
	}

	inline Vector2D QuadLerp(const QuadUV& uv, float x, float y)
	{
		return Vector2D(std::lerp(uv.U1, uv.U2, x), std::lerp(uv.V1, uv.V2, y));
	}
}