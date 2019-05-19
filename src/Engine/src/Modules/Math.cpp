#include "Modules/Math.h"

#include <math.h>

namespace Math
{
	bool AreEqualWithEpsilon(float a, float b, float epsilon)
	{
		return fabs(a - b) < epsilon;
	}
}
