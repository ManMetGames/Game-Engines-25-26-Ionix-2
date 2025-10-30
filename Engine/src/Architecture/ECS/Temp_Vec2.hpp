#pragma once

#include <cmath>
struct Vec2 {
    float x, y;
};

constexpr float PI = 3.14159265358979323846f;
constexpr float DEG2RAD = PI / 180.0f;

//anticlockwise rotation
inline Vec2 Vec2Rotate(Vec2 v, float angle) {
    angle *= DEG2RAD;
    Vec2 result = { 0 };

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.x = v.x * cosres + v.y * sinres;
    result.y = -v.x * sinres + v.y * cosres;

    return result;
}
