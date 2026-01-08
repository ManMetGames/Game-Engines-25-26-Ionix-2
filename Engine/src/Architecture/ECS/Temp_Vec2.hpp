#pragma once

#include <b2_math.h>
#include <cmath>
struct Vec2 {
    float x, y;
};

struct Vec3
{
    float x, y, z;
};


//struct Mat2
//{
//    //a,b
//    //c,d
//    float a, b, c, d;
//};

struct Mat3
{
    //a,b,c
    //d,e,f
    //g,h,i
    float a, b, c, d, e, f, g, h, i;
};

//static Mat2 operator*(const Mat2& left, const Mat2& right)
//{
//    Mat2 output = {
//        (left.a * right.a) + (left.b * right.c),
//        (left.a * right.b) + (left.b * right.d),
//
//        (left.c * right.a) + (left.d * right.c),
//        (left.c * right.b) + (left.d * right.d) };
//
//    return output;
//};

static Mat3 operator*(const Mat3& left, const Mat3& right)
{
    Mat3 output = {
        //row one
        (left.a * right.a) + (left.b * right.d) + (left.c * right.g),
        (left.a * right.b) + (left.b * right.e) + (left.c * right.h),
        (left.a * right.c) + (left.b * right.f) + (left.c * right.i),
        //row two
        (left.d * right.a) + (left.e * right.d) + (left.f * right.g),
        (left.d * right.b) + (left.e * right.e) + (left.f * right.h),
        (left.d * right.c) + (left.e * right.f) + (left.f * right.i),
        //row three
        (left.g * right.a) + (left.h * right.d) + (left.i * right.g),
        (left.g * right.b) + (left.h * right.e) + (left.i * right.h),
        (left.g * right.c) + (left.h * right.f) + (left.i * right.i)
    };
    return output;
}

//static Vec2 operator*(const Vec2& vec, const Mat2& mat)
//{
//    Vec2 output = {
//        (vec.x * mat.a) + (vec.y * mat.b),
//        (vec.x * mat.c) + (vec.y * mat.d) };
//    return output;
//};

static Vec3 operator*(const Vec3& vec, const Mat3& mat)
{
    Vec3 output = {
        (vec.x * mat.a) + (vec.y * mat.b) + (vec.z * mat.c),
        (vec.x * mat.d) + (vec.y * mat.e) + (vec.z * mat.f),
        (vec.x * mat.g) + (vec.y * mat.h) + (vec.z * mat.i) };
    return output;
};

//I was gonna write some sort of explicit cast/operator overload here but I don't know how
static Vec2 FromVec3(Vec3 input)
{
    return Vec2{ input.x, input.y };
};

constexpr float PI = 3.14159265358979323846f;
constexpr float DEG2RAD = PI / 180.0f;

//clockwise rotation
inline Vec2 Vec2Rotate(Vec2 v, float angle) {
    angle *= DEG2RAD;
    Vec2 result = { 0 };

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.x = v.x * cosres + v.y * sinres;
    result.y = -v.x * sinres + v.y * cosres;

    return result;
}




//additional vector functionality from Olesya below 

// normalize a vector
//returns zero vector if its too small
inline Vec2 Vec2Normalize(Vec2 v)
{
    float length = sqrtf(v.x * v.x + v.y * v.y);

    if (length < 0.000001f)
    {
        return Vec2{ 0.0f, 0.0f };
    }

    return Vec2{ v.x / length, v.y / length };
}

inline b2Vec2 b2Vec2Normalize(b2Vec2 v)
{
    float length = sqrtf(v.x * v.x + v.y * v.y);

    if (length < 0.000001f)
    {
        return b2Vec2{ 0.0f, 0.0f };
    }

    return b2Vec2{ v.x / length, v.y / length };
}

inline b2Vec2 b2Lerp(const b2Vec2& a, const b2Vec2& b, float t)
{
    return b2Vec2(a.x + (b.x - a.x) * t,a.y + (b.y - a.y) * t);
}

// scale  vector
inline Vec2 Vec2Scale(Vec2 v, float scalar)
{
    return Vec2{ v.x * scalar, v.y * scalar };
}

// angle between two vectors ( in radians)
inline float Vec2Angle(Vec2 from, Vec2 to)
{

    float magA = sqrtf(from.x * from.x + from.y * from.y);
    float magB = sqrtf(to.x * to.x + to.y * to.y);

    if (magA < 0.000001f || magB < 0.000001f)
    {
        return 0.0f;
    }


    float dot = from.x * to.x + from.y * to.y;
    float v = dot / (magA * magB);

    if (v < -1.0f)
    {
        v = -1.0f;
    }

    else if (v > 1.0f)

    {
        v = 1.0f;
    }

    return acosf(v);
}

// clockwise angle (in radians)
inline float CWAngle(Vec2 v)
{
    if (fabsf(v.x) < 0.000001f && fabsf(v.y) < 0.000001f)
    {

        return 0.0f;
    }

    float angle = atan2f(v.x, v.y);


    if (angle < 0.0f)
    {

        angle += 2.0f * PI;
    }

    return angle;
}

// move the vector towards a target 
inline Vec2 MoveTowards(Vec2 current, Vec2 target, float maxDelta)
{
    Vec2 diff = {

        target.x - current.x,
        target.y - current.y
    };

    float dist = sqrtf(diff.x * diff.x + diff.y * diff.y);



    if (dist < 0.000001f || dist <= maxDelta)
    {
        return target;
    }

    float scale = maxDelta / dist;

    return Vec2{

        current.x + diff.x * scale,
        current.y + diff.y * scale
    };
}

// lerp!
inline Vec2 Vec2Lerp(Vec2 from, Vec2 to, float delta)
{
    if (delta < 0.0f)
    {
        delta = 0.0f;
    }

    else if (delta > 1.0f)
    {
        delta = 1.0f;
    }

    return Vec2{

        from.x + (to.x - from.x) * delta,
        from.y + (to.y - from.y) * delta
    };
}


// smooth interpolation
inline Vec2 Vec2Smooth(Vec2 from, Vec2 to, float delta)
{
    if (delta < 0.0f)
    {
        delta = 0.0f;
    }

    else if (delta > 1.0f)
    {
        delta = 1.0f;
    }

    float smooth = 0.5f + 0.5f * sinf(PI * delta - PI * 0.5f);

    return Vec2{

        from.x + (to.x - from.x) * smooth,
        from.y + (to.y - from.y) * smooth
    };
}

// clamp vector magnitude
inline Vec2 Vec2ClampMagnitude(Vec2 v, float maxMagnitude)
{
    float mag = sqrtf(v.x * v.x + v.y * v.y);

    if (mag < 0.000001f)
    {

        return Vec2{ 0.0f, 0.0f };
    }

    if (mag > maxMagnitude)
    {
        float scale = maxMagnitude / mag;

        return Vec2{ v.x * scale, v.y * scale };
    }

    return v;
}

// clamp vector min and max component values
inline Vec2 Vec2Clamp(Vec2 v, Vec2 min, Vec2 max)
{
    if (v.x < min.x)
    {
        v.x = min.x;
    }

    else if (v.x > max.x)
    {
        v.x = max.x;
    }


    if (v.y < min.y)
    {
        v.y = min.y;
    }

    else if (v.y > max.y)
    {
        v.y = max.y;
    }

    return v;
}

inline Vec2 Min(Vec2 a, Vec2 b)
{
    return Vec2{

        (a.x < b.x) ? a.x : b.x,
        (a.y < b.y) ? a.y : b.y

    };
}

inline Vec2 Max(Vec2 a, Vec2 b)
{
    return Vec2{

        (a.x > b.x) ? a.x : b.x,
        (a.y > b.y) ? a.y : b.y
    };
}