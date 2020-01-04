#pragma once

#include <type_traits>

#include "Base/Debug/Assert.h"

namespace Utils
{
	template<typename I, typename F>
	I CompressNormalizedFloatToIntCL(F unitValue, unsigned int bitsCount)
	{
		static_assert(std::is_integral<I>() && std::is_unsigned<I>(), "CompressNormalizedFloatToIntCL can be used only with unsigned integers");
		static_assert(std::is_floating_point<F>(), "CompressNormalizedFloatToIntCL should be called on floating point");
		Assert(bitsCount <= std::numeric_limits<I>::digits, "bitsCount greater that the real bits count in the corresponding type");

		I maxInterval = (1u << bitsCount) - 1u;
		F scaled = unitValue * static_cast<F>(maxInterval);
		I rounded = static_cast<I>(scaled + 0.5f);

		if (rounded > maxInterval)
		{
			rounded = maxInterval;
		}
		return rounded;
	}

	template<typename F, typename I>
	F DecompressNormalizedFloatFromIntCL(I compressed, unsigned int bitsCount)
	{
		static_assert(std::is_integral<I>() && std::is_unsigned<I>(), "DecompressNormalizedFloatFromIntCL can be used only called on unsigned integers");
		static_assert(std::is_floating_point<F>(), "CompressNormalizedFloatToIntCL should convert value to a floating point");
		Assert(bitsCount <= std::numeric_limits<I>::digits, "bitsCount greater that the real bits count in the corresponding type");

		I maxInterval = (1u << bitsCount) - 1u;
		F intervalSize = 1.0f / static_cast<F>(maxInterval);
		return static_cast<F>(compressed) * intervalSize;
	}

	template<typename I, typename F>
	I CompressFloatToIntCL(F value, F min, F max, unsigned int bitsCount)
	{
		return CompressNormalizedFloatToIntCL<I, F>((value - min)/(max - min), bitsCount);
	}

	template<typename F, typename I>
	F DecompressFloatFromIntCL(I compressed, F min, F max, unsigned int bitsCount)
	{
		return min + DecompressNormalizedFloatFromIntCL<F, I>(compressed, bitsCount) * (max - min);
	}
}
