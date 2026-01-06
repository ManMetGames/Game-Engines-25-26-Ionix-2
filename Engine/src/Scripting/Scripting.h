#pragma once
#include "sol/sol.hpp"
#include "Architecture/Scene.h"

#include "Architecture/Application.h"
#include "LayerSystem/Layer.h"

// Modular binding headers
#include "Scripting/Audio/AudioScripting.h"
#include "Scripting/Window/WindowScripting.h"
#include "Scripting/Input/InputScripting.h"
#include "Scripting/Mafs/MafsScripting.h"
#include "Scripting/Fysics/FysicsScripting.h"
#include "Scripting/Nav/NavScripting.h"
#include "Scripting/Graphics/GraphicsScripting.h"
#include "Scripting/Entity/EntityScripting.h"
#include "Scripting/UI/UIScripting.h"
#include "Scripting/Json/JsonScripting.h"
#include "Scripting/Firebase/FirebaseScripting.h"

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
        void CallHook(const std::string& hookName, Entity* entityA, Entity* entityB);

        sol::state& GetLuaState() { return m_LuaState; }

    private:
        static Scripting* s_Instance;
        sol::state m_LuaState;
    };
}