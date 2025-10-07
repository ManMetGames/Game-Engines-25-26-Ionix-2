#pragma once
#include <corecrt_math_defines.h>

namespace Maf
{
	inline float Deg2Rad(float degrees)
	{
		return degrees * (static_cast<float>(M_PI) / 180.0f);
	}

	inline float Rad2Deg(float radians)
	{
		return radians * (180.0f / static_cast<float>(M_PI));
	}
}
