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
            sol::lib::package,
            sol::lib::coroutine,
            sol::lib::os
        );


        RegisterEngineBindings();

        std::cout << "Lua has been initialised successfully." << std::endl;
    }

    void Scripting::RegisterEngineBindings() {
        MafsScripting::Get().Init(m_LuaState);
        FysicsScripting::Get().Init(m_LuaState);
        WindowScripting::Get().Init(m_LuaState);
        InputScripting::Get().Init(m_LuaState);
        AudioScripting::Get().Init(m_LuaState);
        GraphicsScripting::Get().Init(m_LuaState);
        EntityScripting::Get().Init(m_LuaState);
        UIScripting::Get().Init(m_LuaState);       
    }

    void Scripting::ExecuteScript(const std::string& scriptName) {
        try {
            m_LuaState.script_file(scriptName);
        }
        catch (const sol::error& e) {
            std::cerr << "Lua error while executing script '" << scriptName
                      << "': " << e.what() << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << "Std exception while executing script '" << scriptName
                      << "': " << e.what() << '\n';
        }
        catch (...) {
            std::cerr << "Unknown exception while executing script '"
                      << scriptName << "'\n";
        }
    }

    void Scripting::CallHook(const std::string& hookName) {
        sol::function hook = m_LuaState[hookName];
        if (!hook.valid()) {
            std::cerr << "Hook '" << hookName << "' is invalid\n";
            return;
        }

        try {
            hook();
        }
        catch (const sol::error& e) {
            std::cerr << "Lua error while calling hook '" << hookName
                      << "': " << e.what() << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << "Std exception while calling hook '" << hookName
                      << "': " << e.what() << '\n';
        }
        catch (...) {
            std::cerr << "Unknown exception while calling hook '"
                      << hookName << "'\n";
        }
    }

}