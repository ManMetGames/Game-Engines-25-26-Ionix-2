#pragma once
#include "sol/sol.hpp"

namespace IonixEngine
{
    class LucasScripting
    {
    public:
        static void RegisterMafBindings()
        {
            auto sqrt = [](float x) -> float {
                return Maf::mafSqrt(x);
                };

            auto lerp = [](float x, float y, float t) -> float {
                return Maf::mafLerp(x, y, t);
                };

            auto vector2 = [](float x, float y) -> Maf::mafVector2<float> {
                return Maf::mafVector2(x, y);
                };

            auto vector2x = [](Maf::mafVector2<float> vec2) -> float {
                return vec2.x;
                };

            auto vector2y = [](Maf::mafVector2<float> vec2) -> float {
                return vec2.y;
                };

            auto vector3 = [](float x, float y, float z) -> Maf::mafVector3<float> {
                return Maf::mafVector3(x, y, z);
                };

            auto vector3x = [](Maf::mafVector3<float> vec3) -> float {
                return vec3.x;
                };

            auto vector3y = [](Maf::mafVector3<float> vec3) -> float {
                return vec3.y;
                };

            auto vector3z = [](Maf::mafVector3<float> vec3) -> float {
                return vec3.z;
                };

            Scripting::Get().GetLuaState()["Mafs"] = Scripting::Get().GetLuaState().create_table_with(
                "square_root", sqrt,
                "lerp", lerp,
                "vector2", vector2,
                "vector2_x", vector2x,
                "vector2_y", vector2y,
                "vector3", vector3,
                "vector3_x", vector3x,
                "vector3_y", vector3y,
                "vector3_z", vector3z
            );
        }   

    };
}

