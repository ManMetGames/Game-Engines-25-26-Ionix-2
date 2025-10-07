#pragma once

namespace Maf
{

	template <typename T>
	inline T mafVectorMagnitude(T x, T y)
	{

		return mafSqrt(x * x + y * y);

	};


	template <typename T>
	inline T mafVectorDirection(T x, T y)
	{

		return atan(y / x);

	};

	//Vector with magnitude and direction
	template <typename T>
	struct mafVectorMagDir
	{
		T x, y;
		return { mafVectorMagnitude(x, y), mafVectorDirection(x, y) };

	};


	//Smooth Damp
	template <typename T>
	inline T mafSmoothDamp(T t, T v)
	{
		smoothTime = t;
		yVelocity = v;
		return;

	};


}