#pragma once
#include "sol/sol.hpp"
#include "Architecture/Scene.h"

// Modular binding headers
#include "Scripting/Bindings/AudioBindings.h"
#include "Scripting/Bindings/WindowBindings.h"
#include "Scripting/Bindings/InputBindings.h"
#include "Scripting/Bindings/MafsBindings.h"
#include "Scripting/Bindings/GraphicsBindings.h"
#include "Scripting/Bindings/EntityBindings.h"
#include "Scripting/Bindings/UIBindings.h"

namespace IonixEngine
{
    class Scripting
    {
    public:
        static Scripting& Get();

        void Init();
        void RegisterEngineBindings();
        void ExecuteScript(const std::string& scriptName);
        void CallHook(const std::string& hookName);

        sol::state& GetLuaState() { return m_LuaState; }

    private:
        static Scripting* s_Instance;
        sol::state m_LuaState;
    };
}