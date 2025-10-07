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

	template <typename T>
	inline T mafMax(T x, T y)
	{
		return (x > y) ? x : y;
	}

	template <typename T>
	inline T mafRound(T x)
	{
		return round(x);
	}

	template <typename T>
	inline T mafSqrt(T x)
	{
		return sqrt(x);
	}
}