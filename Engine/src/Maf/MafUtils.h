#pragma once
#include <cmath>
#include <algorithm>
#include <SDL_stdinc.h>

namespace Maf
{
	template <typename T>
	inline T mafAbs(T x)
	{
		return (x < 0) ? -x : x;
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

	//ben mexican implementation

	template <typename T>
	inline T mafSin(T x) {
		return sin(x);
	}
	template <typename T>
	inline T mafCos(T x) {
		return cos(x);
	}
	template <typename T>
	inline T mafTan(T x) {
		return tan(x);
	}
	inline double mafPI() {
		return 3.14159265358979323846264338327950288;
	}

	//Joe - Implementation
	template<typename T>
	inline T mafLerp(T a, T b, T t)
	{
		if (t < 0) t = 0; else if (t > 1) t = 1;
		return a + (b - a) * t;
	}

	template <typename T>
	inline  std::pair<T, T> mafLerpVector(T ax, T ay, T bx, T by, T t) //lerp between 2 points
	{
		T lerpedVectorx;
		T lerpedVectory;
		lerpedVectorx = mafLerp(ax, bx, t);
		lerpedVectory = mafLerp(ay, by, t);
		return std::make_pair(lerpedVectorx, lerpedVectory);
	};

	//template <typename T>
	//struct mafVector2
	//{
	//	T x, y;

	//	mafVector2() : x(0), y(0) {}
	//	mafVector2(T x_, T y_) : x(x_), y(y_) {}

	//	//indexing Vector Implementation
	//	T& operator[](int i) { return (i == 0) ? x : y; }
	//	const T& operator[](int i) const { return (i == 0) ? x : y; }

	//	static mafVector2 maf2Lerp(const mafVector2& a, const mafVector2& b, T t) //using my lerp function to lerp x & y of vector 2
	//	{
	//		return { mafLerp(a.x, b.x, t), mafLerp(a.y, b.y, t) };
	//	}

	//};

	//template <typename T>
	//struct mafVector3
	//{
	//	T x, y, z;

	//	mafVector3() : x(0), y(0), z(0) {}
	//	mafVector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

	//	T& operator[](int i) { return (i == 0) ? x : (i == 1) ? y : z; }
	//	const T& operator[](int i) const { return (i == 0) ? x : (i == 1) ? y : z; }

	//	static mafVector3 maf3Lerp(const mafVector3& a, const mafVector3& b, T t) //using my lerp function to lerp x & y of vector 2
	//	{
	//		return { mafLerp(a.x, b.x, t), mafLerp(a.y, b.y, t), mafLerp(a.z, b.z, t) };
	//	}
	//};

	//Jake - Implementation

	template <typename T>
	inline T mafMod(T x, T y) {
		int a = x;
		int b = y;
		return a % b;
	}

	template <typename T>
	inline T mafDiv(T x, T y) {
		int a = x;
		int b = y;
		return a / b;
	}

	//Josh - Implementation

	template <typename T>
	inline T mafClamp(T x, T min, T max)
	{
		if (x < min) return min;
		if (x > max) return max;
		else return x;
	}

	/*template <typename T>
	inline T mafSqrMagnitudeVec3(const mafVector3<T>& v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}*/
	template <typename T>
	inline T mafSqrMagnitudeVec2(T vx, T vy)
	{
	return vx * vx + vy * vy;
	};

	// ollie implementation 
	 /*
	template <typename T>
	inline T mafVectorMagnitude(T x, T y)
	{
		return mafSqrt(x * x + y * y);
	}

	template <typename T>
	inline T mafVectorDirection(T x, T y)
	{
		return atan(y / x);
	}

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
	}
	*/
	//muhammed implementation
	static inline float Log(float value)
	{
		// avoid log(0) or negative
		value = std::max(value, 1e-37f);
		return std::log(value);
	}

	// logarithm with custom base
	static inline float Log(float value, float base)
	{
		value = std::max(value, 1e-37f);
		base = std::max(base, 1e-37f);
		return std::log(value) / std::log(base);
	}

	// Base-10 logarithm
	static inline float Log10(float value)
	{
		value = std::max(value, 1e-37f);
		return std::log10(value);
	}
	inline float Deg2Rad(float degrees)
	{
		return degrees * (static_cast<float>(mafPI()) / 180.0f);
	}

	inline float Rad2Deg(float radians)
	{
		return radians * (180.0f / static_cast<float>(mafPI()));
	}

	//additional vector functions from Olesya


}
