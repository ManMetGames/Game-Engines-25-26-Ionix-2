#pragma once
#include "sol/sol.hpp"

namespace IonixEngine
{
    class Scripting
    {
    public:
        //singleton
        static Scripting& Get();

        //Core
        void Init();

        // Registration Methods
        void RegisterEngineBindindings();

        void ExecuteScript(const std::string& scriptName);

        void GetWindowHeight();

        void GetWindowWidth();

        void CallHook(const std::string& hookName);
    private:
        //singleton Instance
        static Scripting* s_Instance;
        sol::state m_LuaState;


        void RegisterWindowBindindings();

    };
}

