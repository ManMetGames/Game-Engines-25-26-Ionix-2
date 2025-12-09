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
        auto drawLabel = [](const char* text, int xsize, int ysize, int xpos, int ypos, const char* font) {
            Application::Get().layerUI->m_UI->DrawLabel((char*)text, xsize, ysize, xpos, ypos, "");
            };
        auto DrawButton = [](const char* text, int xsize, int ysize, int xpos, int ypos) -> bool {
            return Application::Get().layerUI->m_UI->DrawButton((char*)text, xsize, ysize, xpos, ypos);
            };
        auto AddButton = [](int x, int y, float xSize, float ySize, const char* text, std::function<void()> onClick) {
            return Application::Get().layerUI->m_UIManager->AddButton(x, y, xSize, ySize, text, onClick);
            };
        
        auto AddSlider = [](const char* label, float value, int xsize, int ysize, int xpos, int ypos, float minval, float maxval) -> float {
            return Application::Get().layerUI->m_UI->AddSlider((char*)label, value, xsize, ysize, xpos, ypos, minval, maxval);
            };
		auto AddColorPicker = [](int x, int y, float xSize, float ySize, const char* label, float* color) {
			return Application::Get().layerUI->m_UI->DrawColorPicker(x, y, xSize, ySize, label, color);
			};
        auto AddDropdown = [](int x, int y, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex) {
            Application::Get().layerUI->m_UIManager->AddDropdown(x, y, xSize, ySize, text, options, currentIndex);
            };
        auto AddRadioButton = [](int x, int y, float xSize, float ySize, const char* text, int* radioValuePointer, int value, bool sameline) {
            Application::Get().layerUI->m_UIManager->AddRadioButton(x, y, xSize, ySize, text, radioValuePointer, value, sameline);
            };
        auto AddInputText = [](int x, int y, float xSize, float ySize, const char* text, char* buffer, size_t bufferSize) {
            Application::Get().layerUI->m_UIManager->AddInputText(x, y, xSize, ySize, text, buffer, bufferSize);
            };
        auto AddSliderFloat = [](int x, int y, float xSize, float ySize,const char* text, float* value,float min, float max) {
            Application::Get().layerUI->m_UIManager->AddSliderFloat(x, y, xSize, ySize,text, value, min, max);
            };
        auto AddCheckbox = [](int x, int y, float xSize, float ySize, const char* text, bool* checked) {
            Application::Get().layerUI->m_UIManager->AddCheckbox(x, y, xSize, ySize, text, checked);
            };
 
        lua["UI"] = lua.create_table_with(
          "Add_label", AddLabel,
			    "draw_label", drawLabel,
          "draw_button", DrawButton,
			    "add_button", AddButton,
			    "add_slider", AddSlider,
			"add_color_picker", AddColorPicker,
			"add_drop_down", AddDropdown,
            "add_radio_button", AddRadioButton,
            "add_input_text", AddInputText,
            "add_slider_float",AddSliderFloat,
            "add_checkbox ", AddCheckbox
        );
    }

}