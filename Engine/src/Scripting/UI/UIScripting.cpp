#include "Scripting/UI/UIScripting.h"
#include "Architecture/Application.h"

namespace IonixEngine {
    UIScripting* UIScripting::s_Instance = nullptr;

    UIScripting& UIScripting::Get() {
        if (!s_Instance)
            s_Instance = new UIScripting();
        return *s_Instance;
    }

    void UIScripting::Init(sol::state& lua)
    {
        auto drawLabel = [](const char* text, int xsize, int ysize, int xpos, int ypos, const char* font) {
            Application::Get().layerUI->m_UI->DrawLabel((char*)text, xsize, ysize, xpos, ypos, "");
            };

        auto drawButton = [](const char* text, int xsize, int ysize, int xpos, int ypos) -> bool {
            return Application::Get().layerUI->m_UI->DrawButton((char*)text, xsize, ysize, xpos, ypos);
            };

        auto addSlider = [](const char* label, float value, int xsize, int ysize, int xpos, int ypos, float minval, float maxval) -> float {
            return Application::Get().layerUI->m_UI->AddSlider((char*)label, value, xsize, ysize, xpos, ypos, minval, maxval);
            };

        lua["UI"] = lua.create_table_with(
            "draw_label", drawLabel,
            "draw_button", drawButton,
            "add_slider", addSlider
        );
    }

}