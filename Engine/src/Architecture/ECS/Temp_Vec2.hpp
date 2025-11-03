#pragma once

#include <cmath>
struct Vec2 {
    float x, y;
};

struct Vec3
{
    float x, y, z;
};


struct Mat2
{
    //a,b
    //c,d
    float a, b, c, d;
};

struct Mat3
{
    //a,b,c
    //d,e,f
    //g,h,i
    float a, b, c, d, e, f, g, h, i;
};

static Mat2 operator*(const Mat2& left, const Mat2& right)
{
    Mat2 output = {
        (left.a * right.a) + (left.b * right.c),
        (left.a * right.b) + (left.b * right.d),

        (left.c * right.a) + (left.d * right.c),
        (left.c * right.b) + (left.d * right.d) };

    return output;
};

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

static Vec2 operator*(const Vec2& vec, const Mat2& mat)
{
    Vec2 output = {
        (vec.x * mat.a) + (vec.y * mat.b),
        (vec.x * mat.c) + (vec.y * mat.d) };
    return output;
};

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