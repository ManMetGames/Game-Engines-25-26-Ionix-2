#pragma once
#include "sol/sol.hpp"

namespace IonixEngine
{
    class Scripting
    {
    public:
        //Singleton
        static Scripting& Get();

        //Core
        void Init();
    private:
        //Singleton Instance
        static Scripting* s_Instance;

        sol::state m_LuaState;
    };
}

