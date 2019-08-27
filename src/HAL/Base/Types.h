#pragma once

#include <cstdint>

namespace Graphics
{
	struct QuadUV
	{
		float U1 = 0.0f;
		float V1 = 0.0f;
		float U2 = 1.0f;
		float V2 = 1.0f;
	};

	struct Color
	{
		std::uint8_t R;
		std::uint8_t G;
		std::uint8_t B;
		std::uint8_t A;
	};
}
