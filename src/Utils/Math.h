#pragma once

#include <cmath>

// delete this code when C++20 is fully supported
#if __cplusplus <= 201703L
namespace std
{
	inline float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}
}
#endif

namespace Math
{
	bool AreEqualWithEpsilon(float a, float b, float epsilon);
}
