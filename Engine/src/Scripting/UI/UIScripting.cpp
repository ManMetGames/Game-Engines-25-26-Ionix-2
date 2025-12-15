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
        auto AddLabel = [](const int x, int y, float xSize, float ySize, const char* text, sol::optional<std::string> fontName,
            sol::optional<float> fontScale) {
            Application::Get().layerUI->m_UIManager->AddLabel(x, y, xSize, ySize, text,
                fontName.value_or(""),
                fontScale.value_or(1.0f));
            };

        auto AddCenteredLabel = [](float centerX, float y, const char* text, sol::optional<std::string> fontName,
            sol::optional<float> fontScale) {
                Application::Get().layerUI->m_UIManager->AddCenteredLabel(centerX, y, text,
                    fontName.value_or(""),
                    fontScale.value_or(1.0f));
            };

        auto AddButton = [](int x, int y, float w, float h, const char* text, sol::optional<std::string> id,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale)
            {
                Application::Get().layerUI->m_UIManager->AddButton(
                    x, y, w, h, text,
                    id ? id->c_str() : nullptr,
                    fontName.value_or(""),
                    fontScale.value_or(1.0f)
                );
            };

        auto WasButtonPressed = [](const std::string& id)
            {
                return Application::Get().layerUI->m_UIManager->WasButtonPressed(id);
            };

        auto AddCheckbox = [](int x, int y, float w, float h,
            const char* text, const std::string& id,
            sol::optional<bool> defaultValue, sol::optional<std::string> fontName,
            sol::optional<float> fontScale)
            {
                Application::Get().layerUI->m_UIManager->AddCheckboxID(
                    x, y, w, h, text, id.c_str(),
                    defaultValue.value_or(false),
                    fontName.value_or(""),
                    fontScale.value_or(1.0f)
                );
            };

        auto GetCheckbox = [](const std::string& id)
            {
                return Application::Get().layerUI->m_UIManager->GetCheckbox(id);
            };

        auto WasCheckboxChanged = [](const std::string& id)
            {
                return Application::Get().layerUI->m_UIManager->WasCheckboxChanged(id);
            };

        auto AddSliderFloat = [](int x, int y, float width,
            const char* label, const std::string& id,
            float min, float max,
            sol::optional<float> defaultValue, sol::optional<std::string> fontName,
            sol::optional<float> fontScale)
            {
                Application::Get().layerUI->m_UIManager->AddSliderFloat(
                    x, y, width,
                    label, id.c_str(),
                    min, max,
                    defaultValue.value_or(0.0f),
                    fontName.value_or(""),
                    fontScale.value_or(1.0f)
                );
            };

        auto GetSlider = [](const std::string& id)
            {
                return Application::Get().layerUI->m_UIManager->GetSlider(id);
            };

        auto WasSliderChanged = [](const std::string& id)
            {
                return Application::Get().layerUI->m_UIManager->WasSliderChanged(id);
            };

        auto SetSlider = [](const std::string& id, float v)
            {
                Application::Get().layerUI->m_UIManager->SetSlider(id, v);
            };

		//auto AddColorPicker = [](int x, int y, float xSize, float ySize, const char* label, float* color) {
		//	return Application::Get().layerUI->m_UI->DrawColorPicker(x, y, xSize, ySize, label, color);
		//	};
        auto DrawProgressBar = [](int x, int y, float xSize, float ySize, float maxValue, float currentValue, int colorId) {
            Application::Get().layerUI->m_UI->DrawProgressBar(x, y, xSize, ySize, maxValue, currentValue, colorId);
            };
        auto AddInputText = [](int xPos, int yPos, float width, const char* label, const char* id, size_t maxLen, sol::optional<std::string> fontName,
            sol::optional<float> fontScale) {
            Application::Get().layerUI->m_UIManager->AddInputText(xPos, yPos, width, label, id, maxLen,
                fontName.value_or(""),
                fontScale.value_or(1.0f));
			};

        auto GetInputText = [](const std::string& id) {
            return Application::Get().layerUI->m_UIManager->GetCommittedText(id);
            };

        auto WasInputCommitted = [](const std::string& id) {
            return Application::Get().layerUI->m_UIManager->WasInputCommitted(id);
            };

        auto ClearInput = [](const std::string& id) {
            Application::Get().layerUI->m_UIManager->ClearInput(id);
            };

        auto AddPanel = [](int x, int y, float w, float h,
            float alpha, float rounding,
            sol::optional<int> r, sol::optional<int> g, sol::optional<int> b)
            {
                int R = r.value_or(0);
                int G = g.value_or(0);
                int B = b.value_or(0);
                Application::Get().layerUI->m_UIManager->AddPanel(x, y, w, h, alpha, rounding, R, G, B);
            };

        auto CalcTextWidth = [](const char* text) {
            return ImGui::CalcTextSize(text).x;
            };


        lua["UI"] = lua.create_table_with(
            "Add_label", AddLabel,
            "add_centered_label", AddCenteredLabel,
            "add_button", AddButton,
            "add_checkbox", AddCheckbox,
            "add_slider_float", AddSliderFloat,
            //"add_color_picker", AddColorPicker,
            "draw_progress_bar", DrawProgressBar,
            "add_input_text", AddInputText,
            "add_panel", AddPanel,

            "was_button_pressed", WasButtonPressed,
            "get_checkbox", GetCheckbox,
            "was_checkbox_changed", WasCheckboxChanged,
            "get_slider", GetSlider,
            "was_slider_changed", WasSliderChanged,
            "set_slider", SetSlider,
			"get_input_text", GetInputText,
            "was_input_committed", WasInputCommitted,
            "clear_input", ClearInput,
            "calc_text_width", CalcTextWidth
        );
    }
}