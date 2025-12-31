#include "Scripting/Window/WindowScripting.h"
#include "Architecture/Application.h"
#include <SDL.h>

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
        auto setWindowSize = [](std::int32_t width, std::int32_t height) {
            Window& window = Application::Get().GetWindow();
            SDL_SetWindowSize(window.m_Window, width, height);
            window.m_Data.Width = width;
            window.m_Data.Height = height;
            };
        auto setWindowSizeCentered = [](std::int32_t width, std::int32_t height) {
            Window& window = Application::Get().GetWindow();
            int oldW, oldH, x, y;
            SDL_GetWindowSize(window.m_Window, &oldW, &oldH);
            SDL_GetWindowPosition(window.m_Window, &x, &y);
            int newX = x + (oldW - width) / 2;
            int newY = y + (oldH - height) / 2;
            SDL_SetWindowSize(window.m_Window, width, height);
            SDL_SetWindowPosition(window.m_Window, newX, newY);
            window.m_Data.Width = width;
            window.m_Data.Height = height;
            };
        auto setWindowPos = [](std::int32_t x, std::int32_t y) {
            Window& window = Application::Get().GetWindow();
            SDL_SetWindowPosition(window.m_Window, x, y);
            };
        auto getWindowPos = [&lua]() -> sol::table {
            Window& window = Application::Get().GetWindow();
            int x, y;
            SDL_GetWindowPosition(window.m_Window, &x, &y);
            sol::table pos = lua.create_table();
            pos["x"] = x;
            pos["y"] = y;
            return pos;
            };
        auto getDisplayWidth = []() -> std::int32_t {
            SDL_DisplayMode mode;
            if (SDL_GetCurrentDisplayMode(0, &mode) == 0) {
                return mode.w;
            }
            return 1920;
            };
        auto getDisplayHeight = []() -> std::int32_t {
            SDL_DisplayMode mode;
            if (SDL_GetCurrentDisplayMode(0, &mode) == 0) {
                return mode.h;
            }
            return 1080;
            };

        auto quitWindow = []() {
            Window& window = Application::Get().GetWindow();

            // Simulate clicking the window X button
            SDL_Event e{};
            e.type = SDL_WINDOWEVENT;
            e.window.event = SDL_WINDOWEVENT_CLOSE;
            e.window.windowID = SDL_GetWindowID(window.m_Window);
            SDL_PushEvent(&e);
            };


        lua["Window"] = lua.create_table_with(
            "get_title", getWindowTitle,
            "get_width", getWindowWidth,
            "get_height", getWindowHeight,
            "set_size", setWindowSize,
            "set_size_centered", setWindowSizeCentered,
            "set_pos", setWindowPos,
            "get_pos", getWindowPos,
            "get_display_width", getDisplayWidth,
            "get_display_height", getDisplayHeight,
            "quit", quitWindow

        );
    }
}