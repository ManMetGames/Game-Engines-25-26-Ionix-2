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

            Scripting::Get().GetLuaState()["Mafs"] = Scripting::Get().GetLuaState().create_table_with(
                "square_root", sqrt
            );
        }   

    };
}

