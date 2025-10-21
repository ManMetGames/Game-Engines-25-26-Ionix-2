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
        void RegisterEngineBindings();

        void ExecuteScript(const std::string& scriptName); 

        void CallHook(const std::string& hookName);

        sol::state& GetLuaState() { return m_LuaState; }


    private:
        //singleton Instance
        static Scripting* s_Instance;
        sol::state m_LuaState;


        void RegisterWindowBindings();

        void RegisterInputBindings();

        void RegisterMafsBindings();

        void RegisterGraphicsBindings();

    };
}

