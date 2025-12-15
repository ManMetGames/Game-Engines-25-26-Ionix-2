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
                Application::Get().layerUI->m_UIManager->AddCheckbox(
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

        auto AddSlider = [](int x, int y, float width,
            const char* label, const std::string& id,
            float min, float max,
            sol::optional<float> defaultValue, sol::optional<std::string> fontName,
            sol::optional<float> fontScale)
            {
                Application::Get().layerUI->m_UIManager->AddSlider(
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

        auto AddRadio = [](int x, int y, float w, float h, const char* label,
            const std::string& groupId, int value,
            sol::optional<int> defaultValue,
            sol::optional<bool> sameline,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale)
            {
                Application::Get().layerUI->m_UIManager->AddRadioToggle(
                    x, y, w, h, label,
                    groupId.c_str(), value,
                    defaultValue.value_or(0),
                    sameline.value_or(false),
                    fontName.value_or(""),
                    fontScale.value_or(1.0f)
                );
            };

        auto GetRadio = [](const std::string& groupId)
            {
                return Application::Get().layerUI->m_UIManager->GetRadio(groupId);
            };

        auto WasRadioChanged = [](const std::string& groupId)
            {
                return Application::Get().layerUI->m_UIManager->WasRadioChanged(groupId);
            };

        auto AddDropdown = [](int x, int y, float w, float h, const char* label,
            const std::string& id, sol::table options,
            sol::optional<int> defaultIndex,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale)
            {
                std::vector<std::string> opts;
                for (auto& kv : options)
                {
                    sol::object v = kv.second;
                    if (v.is<std::string>()) opts.push_back(v.as<std::string>());
                }

                Application::Get().layerUI->m_UIManager->AddDropdown(
                    x, y, w, h, label,
                    id.c_str(), opts,
                    defaultIndex.value_or(0),
                    fontName.value_or(""),
                    fontScale.value_or(1.0f)
                );
            };

        auto GetDropdownIndex = [](const std::string& id)
            {
                return Application::Get().layerUI->m_UIManager->GetDropdownIndex(id);
            };

        auto WasDropdownChanged = [](const std::string& id)
            {
                return Application::Get().layerUI->m_UIManager->WasDropdownChanged(id);
            };

        auto AddColorPicker = [](int x, int y, float w, float h, const char* label,
            const std::string& id,
            sol::optional<float> r, sol::optional<float> g,
            sol::optional<float> b, sol::optional<float> a,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale)
            {
                ImVec4 def(r.value_or(1.0f), g.value_or(1.0f), b.value_or(1.0f), a.value_or(1.0f));

                Application::Get().layerUI->m_UIManager->AddColorPicker(
                    x, y, w, h, label,
                    id.c_str(), def,
                    fontName.value_or(""),
                    fontScale.value_or(1.0f)
                );
            };

        auto GetColor = [](sol::this_state s, const std::string& id)
            {
                ImVec4 c = Application::Get().layerUI->m_UIManager->GetColor(id);
                sol::state_view lua(s);
                sol::table t = lua.create_table_with(1, c.x, 2, c.y, 3, c.z, 4, c.w);
                return t;
            };

        auto WasColorChanged = [](const std::string& id)
            {
                return Application::Get().layerUI->m_UIManager->WasColorChanged(id);
            };

        auto BeginChild = [](int x, int y, float w, float h, const std::string& id,
            sol::optional<bool> border,
            sol::optional<int> flags)
            {
                Application::Get().layerUI->m_UIManager->BeginChild(
                    x, y, w, h,
                    id.c_str(),
                    border.value_or(false),
                    (ImGuiWindowFlags)flags.value_or(0)
                );
            };

        auto EndChild = []()
            {
                Application::Get().layerUI->m_UIManager->EndChild();
            };


        lua["UI"] = lua.create_table_with(
            "Add_label", AddLabel,
            "add_centered_label", AddCenteredLabel,
            "add_button", AddButton,
            "add_checkbox", AddCheckbox,
            "add_slider", AddSlider,
            "draw_progress_bar", DrawProgressBar,
            "add_input_text", AddInputText,
            "add_panel", AddPanel,
            "add_radio", AddRadio,
            "add_dropdown", AddDropdown,
            "add_color_picker", AddColorPicker,

            "was_button_pressed", WasButtonPressed,
            "get_checkbox", GetCheckbox,
            "was_checkbox_changed", WasCheckboxChanged,
            "get_slider", GetSlider,
            "was_slider_changed", WasSliderChanged,
            "set_slider", SetSlider,
			"get_input_text", GetInputText,
            "was_input_committed", WasInputCommitted,
            "get_radio", GetRadio,
            "was_radio_changed", WasRadioChanged,
            "get_dropdown_index", GetDropdownIndex,
            "was_dropdown_changed", WasDropdownChanged,
            "get_color", GetColor,
            "was_color_changed", WasColorChanged,
            "clear_input", ClearInput,
            "calc_text_width", CalcTextWidth,
            "begin_child", BeginChild,
            "end_child", EndChild
        );
    }
}