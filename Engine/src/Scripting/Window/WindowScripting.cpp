#include "Scripting/Window/WindowScripting.h"
#include "Architecture/Application.h"

namespace IonixEngine {
    WindowScripting* WindowScripting::s_Instance = nullptr;

    WindowScripting& WindowScripting::Get() {
        if (!s_Instance)
            s_Instance = new WindowScripting();
        return *s_Instance;
    }

    void WindowScripting::Init(sol::state& lua)
    {
        auto getWindowTitle = []() -> std::string {
            return Application::Get().GetWindow().m_Data.Title;
            };
        auto getWindowWidth = []() -> std::int32_t {
            return Application::Get().GetWindow().m_Data.Width;
            };
        auto getWindowHeight = []() -> std::int32_t {
            return Application::Get().GetWindow().m_Data.Height;
            };

        lua["Window"] = lua.create_table_with(
            "get_title", getWindowTitle,
            "get_width", getWindowWidth,
            "get_height", getWindowHeight
        );
    }
}