#pragma once

namespace Maf
{
	template <typename T>
	inline T mafAbs(T x)
	{
		return (x < 0) ?  -x : x;
	}

	template <typename T>
	inline T mafMin(T x, T y)
	{
		return (x < y) ? x : y;
	}
}