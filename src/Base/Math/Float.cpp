#include "Base/precomp.h"

#include "Base/Math/Float.h"

#include <math.h>
#include <memory.h>

namespace Math
{
#ifdef DEBUG
	static void CheckFloatsCanBeCompared(float a, float b, float epsilon)
	{
		Assert(isfinite(a) && isfinite(b), "Comparison with NaN or Inf is not allowed");
		Assert(isfinite(epsilon), "Epsilon should not be NaN or Inf");
		Assert(std::ilogb(a) < std::ilogb(epsilon) + 27, "Number is too big to be compared with this epsilon: %f %f", a, epsilon);
		Assert(std::ilogb(b) < std::ilogb(epsilon) + 27, "Number is too big to be compared with this epsilon: %f %f", b, epsilon);
	}
#endif // DEBUG

	bool AreEqualWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG
		return fabs(a - b) < epsilon;
	}

	bool IsGreaterWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG
		return a > b + epsilon;
	}

	bool IsGreaterOrEqualWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG
		return a + epsilon >= b;
	}

	bool IsLessWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG
		return a < b - epsilon;
	}

	bool IsLessOrEqualWithEpsilon(float a, float b, float epsilon)
	{
#ifdef DEBUG
		CheckFloatsCanBeCompared(a, b, epsilon);
#endif // DEBUG
		return a - epsilon <= b;
	}
}
