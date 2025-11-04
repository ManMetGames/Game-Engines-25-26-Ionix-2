#pragma once
#include "sol/sol.hpp"
#include "Architecture/Scene.h"

#include "Architecture/Application.h"
#include "LayerSystem/Layer.h"

// Modular binding headers
#include "Scripting/Audio/AudioBindings.h"
#include "Scripting/Window/WindowBindings.h"
#include "Scripting/Input/InputBindings.h"
#include "Scripting/Mafs/MafsBindings.h"
#include "Scripting/Graphics/GraphicsBindings.h"
#include "Scripting/Entity/EntityBindings.h"
#include "Scripting/UI/UIBindings.h"

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