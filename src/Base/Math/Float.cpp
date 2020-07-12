#include "Base/precomp.h"

#include "Base/Math/Float.h"

#include <math.h>
#include <memory.h>

namespace Math
{
#ifdef DEBUG_CHECKS
	static void CheckFloatsCanBeCompared(float a, float b, float epsilon)
	{
		Assert(isfinite(a) && isfinite(b), "Comparison with NaN or Inf is not allowed");
		Assert(isfinite(epsilon), "Epsilon should not be NaN or Inf");
		Assert(std::ilogb(a) < std::ilogb(epsilon) + 27, "Number is too big to be compared with this epsilon: %f %f", a, epsilon);
		Assert(std::ilogb(b) < std::ilogb(epsilon) + 27, "Number is too big to be compared with this epsilon: %f %f", b, epsilon);
	}
#endif // DEBUG_CHECKS

	bool AreEqualWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG_CHECKS
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG_CHECKS
		return fabs(a - b) < epsilon;
	}

	bool IsGreaterWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG_CHECKS
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG_CHECKS
		return a > b + epsilon;
	}

	bool IsGreaterOrEqualWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG_CHECKS
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG_CHECKS
		return a + epsilon >= b;
	}

	bool IsLessWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG_CHECKS
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG_CHECKS
		return a < b - epsilon;
	}

	bool IsLessOrEqualWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG_CHECKS
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG_CHECKS
		return a - epsilon <= b;
	}
}
