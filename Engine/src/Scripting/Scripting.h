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

        //Registration methods
        void RegisterEngineBindings();

        void ExecuteScript(const std::string& scriptName);

    private:
        //Singleton Instance
        static Scripting* s_Instance;
        sol::state m_LuaState;

        void RegisterWindowBindings();
    };
}

