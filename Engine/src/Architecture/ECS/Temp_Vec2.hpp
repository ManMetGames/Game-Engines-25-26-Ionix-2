#pragma once

#include <cmath>
struct Vec2 {
    float x, y;
};

struct Mat2
{
    //a,b
    //c,d
    float a, b, c, d;
};

//Vec2 operator*(Vec2 vec, Mat2 mat)
//{
//    Vec2 output = {
//        (vec.x * mat.a) + (vec.y * mat.b),
//        (vec.x * mat.c) + (vec.y * mat.d) };
//    return output;
//}

constexpr float PI = 3.14159265358979323846f;
constexpr float DEG2RAD = PI / 180.0f;

//clockwise rotation
inline Vec2 Vec2Rotate(Vec2 v, float angle) {
    angle *= DEG2RAD;
    Vec2 result = { 0 };

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.x = v.x*cosres + v.y*sinres;
    result.y = -v.x*sinres + v.y*cosres;

    return result;
}
