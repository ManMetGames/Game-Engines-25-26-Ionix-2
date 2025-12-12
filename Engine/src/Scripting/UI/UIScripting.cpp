#include "Scripting/UI/UIScripting.h"
#include "UI/UIManager.h"
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
        auto AddLabel = [](const int x, int y, float xSize, float ySize, const char* text) {
            Application::Get().layerUI->m_UIManager->AddLabel(x, y, xSize, ySize, text);
            };
        auto AddButton = [](int x, int y, float xSize, float ySize, const char* text) {
            return Application::Get().layerUI->m_UIManager->AddButton(x, y, xSize, ySize, text);
            };
        //auto AddSlider = [](const char* label, float value, int xsize, int ysize, int xpos, int ypos, float minval, float maxval) -> float {
            //return Application::Get().layerUI->m_UI->AddSlider((char*)label, value, xsize, ysize, xpos, ypos, minval, maxval);
            //};
		auto AddColorPicker = [](int x, int y, float xSize, float ySize, const char* label, float* color) {
			return Application::Get().layerUI->m_UI->DrawColorPicker(x, y, xSize, ySize, label, color);
			};
 
        lua["UI"] = lua.create_table_with(
          "Add_label", AddLabel,
			    "add_button", AddButton,
			    //"add_slider", AddSlider,
			"add_color_picker", AddColorPicker
			
        );
    }

}