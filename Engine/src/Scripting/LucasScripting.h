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

            auto vector2x = [](Maf::mafVector2<float> vec2) -> float
                {
                    return vec2.x;
                };

            auto vector2y = [](Maf::mafVector2<float> vec2) -> float
                {
                    return vec2.y;
                };

            Scripting::Get().GetLuaState()["Mafs"] = Scripting::Get().GetLuaState().create_table_with(
                "square_root", sqrt,
                "lerp", lerp,
                "vector2", vector2,
                "vector2_x", vector2x,
                "vector2_y", vector2y
            );
        }   

    };
}

