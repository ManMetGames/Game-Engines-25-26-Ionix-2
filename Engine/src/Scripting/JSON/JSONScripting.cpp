#include "Scripting/UI/UIScripting.h"
#include "UI/UIManager.h"
#include "Architecture/Application.h"
#include "Scripting/JSON/JSONScripting.h"

namespace IonixEngine {
    JSONScripting* JSONScripting::s_Instance = nullptr;

    JSONScripting& JSONScripting::Get() {
        if (!s_Instance)
            s_Instance = new JSONScripting();
        return *s_Instance;
    }

    void JSONScripting::Init(sol::state& lua)
    {
        auto saveHighScore = []() {};
        auto loadHighScore = []() {};
        /*
        auto AddLabel = [](const int x, int y, float xSize, float ySize, const char* text) {
            Application::Get().layerUI->m_UIManager->AddLabel(x, y, xSize, ySize, text);
            };
        auto drawLabel = [](const char* text, int xsize, int ysize, int xpos, int ypos, const char* font) {
            Application::Get().layerUI->m_UI->DrawLabel((char*)text, xsize, ysize, xpos, ypos, "");
            };
        */
        lua["JSON"] = lua.create_table_with(
            //"Add_label", AddLabel

        );
    }

}