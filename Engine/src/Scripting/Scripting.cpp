#include "Scripting/Scripting.h"

namespace IonixEngine {

    Scripting* Scripting::s_Instance = nullptr;

    Scripting& Scripting::Get() {
        if (!s_Instance) {
            s_Instance = new Scripting();
        }
        return *s_Instance;
    }

    void Scripting::Init() {
        std::cout << "Lua is now being initialized." << std::endl;

        m_LuaState.open_libraries(
            sol::lib::base,
            sol::lib::string,
            sol::lib::math,
            sol::lib::table,
            sol::lib::io,
            sol::lib::package
        );


        RegisterEngineBindings();

        std::cout << "Lua has been initialised successfully." << std::endl;
    }

    void Scripting::RegisterEngineBindings() {
        WindowScripting::Get().Init(m_LuaState);
        InputScripting::Get().Init(m_LuaState);
        AudioScripting::Get().Init(m_LuaState);
        GraphicsScripting::Get().Init(m_LuaState);
        EntityScripting::Get().Init(m_LuaState);
        UIScripting::Get().Init(m_LuaState);
        MafsScripting::Get().Init(m_LuaState);
    }

    void Scripting::ExecuteScript(const std::string& scriptName) {
        m_LuaState.script_file(scriptName);
    }

    void Scripting::CallHook(const std::string& hookName) {
        sol::function hook = m_LuaState[hookName];
        if (hook.valid()) {
            try {
                hook();
            }
            catch (const std::exception& e) {
                std::cerr << "Error calling hook '" << hookName << "': " << e.what() << '\n';
            }
        }
        else {
            std::cerr << "Hook '" << hookName << "' is invalid\n";
        }
    }

}