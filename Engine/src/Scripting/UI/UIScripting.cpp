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
		auto AddColorPicker = [](int x, int y, float xSize, float ySize, const char* label, float* color) {
			return Application::Get().layerUI->m_UI->DrawColorPicker(x, y, xSize, ySize, label, color);
			};
        auto DrawProgressBar = [](int x, int y, float xSize, float ySize, float maxValue, float currentValue, int colorId) {
            Application::Get().layerUI->m_UI->DrawProgressBar(x, y, xSize, ySize, maxValue, currentValue, colorId);
            };
        auto AddInputText = [](int xPos, int yPos, float width, const char* label, const char* id, size_t maxLen) {
            Application::Get().layerUI->m_UIManager->AddInputText(xPos, yPos, width, label, id, maxLen);
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
        auto AddCenteredLabel = [](float centerX, float y, const char* text) {
            Application::Get().layerUI->m_UIManager->AddCenteredLabel(centerX, y, text);
            };

        lua["UI"] = lua.create_table_with(
            "Add_label", AddLabel,
            "add_button", AddButton,
            "add_color_picker", AddColorPicker,
            "draw_progress_bar", DrawProgressBar,
            "add_input_text", AddInputText,
            "add_panel", AddPanel,

			"get_input_text", GetInputText,
            "was_input_committed", WasInputCommitted,
            "clear_input", ClearInput,
            "calc_text_width", CalcTextWidth,
            "add_centered_label", AddCenteredLabel
        );
    }
}