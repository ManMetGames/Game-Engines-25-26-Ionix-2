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
            sol::optional<float> fontScale, sol::optional<float> wrapWidth) {
                Application::Get().layerUI->m_UIManager->AddLabel(x, y, xSize, ySize, text,
                    fontName.value_or(""),
                    fontScale.value_or(1.0f),
                    wrapWidth.value_or(0.0f));
            };

        auto AddCenteredLabel = [](float centerX, float y, const char* text, sol::optional<std::string> fontName,
            sol::optional<float> fontScale, sol::optional<float> wrapWidth) {
                Application::Get().layerUI->m_UIManager->AddCenteredLabel(centerX, y, text,
                    fontName.value_or(""),
                    fontScale.value_or(1.0f),
                    wrapWidth.value_or(0.0f));
            };



        auto AddLabelColored = [](const int x, int y, float xSize, float ySize, const char* text,
            sol::table color,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale, sol::optional<float> wrapWidth)
            {
                auto norm = [](float v) -> float { return (v > 1.0f) ? (v / 255.0f) : v; };

                auto get = [&](const char* key, int idx, float def) -> float {
                    if (sol::optional<float> v = color[idx]) return *v;
                    if (sol::optional<float> v = color[key]) return *v;
                    return def;
                    };

                float r = norm(get("r", 1, 1.0f));
                float g = norm(get("g", 2, 1.0f));
                float b = norm(get("b", 3, 1.0f));
                float a = norm(get("a", 4, 1.0f));

                Application::Get().layerUI->m_UIManager->AddLabelColored(
                    x, y, xSize, ySize, text,
                    r, g, b, a,
                    fontName.value_or(""),
                    fontScale.value_or(1.0f),
                    wrapWidth.value_or(0.0f)
                );
            };

        auto AddCenteredLabelColored = [](float centerX, float y, const char* text,
            sol::table color,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale, sol::optional<float> wrapWidth)
            {
                auto norm = [](float v) -> float { return (v > 1.0f) ? (v / 255.0f) : v; };

                auto get = [&](const char* key, int idx, float def) -> float {
                    if (sol::optional<float> v = color[idx]) return *v;
                    if (sol::optional<float> v = color[key]) return *v;
                    return def;
                    };

                float r = norm(get("r", 1, 1.0f));
                float g = norm(get("g", 2, 1.0f));
                float b = norm(get("b", 3, 1.0f));
                float a = norm(get("a", 4, 1.0f));

                Application::Get().layerUI->m_UIManager->AddCenteredLabelColored(
                    centerX, y, text,
                    r, g, b, a,
                    fontName.value_or(""),
                    fontScale.value_or(1.0f),
                    wrapWidth.value_or(0.0f)
                );
            };

        auto AddButton = [](int x, int y, float w, float h, const char* text,
            sol::optional<std::string> id,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale,
            sol::optional<float> rounding,
            sol::optional<bool> useColor,
            sol::optional<float> r, sol::optional<float> g,
            sol::optional<float> b, sol::optional<float> a)
            {
                auto norm = [](float v) -> float
                    {
                        return (v > 1.0f) ? (v / 255.0f) : v;
                    };

                Application::Get().layerUI->m_UIManager->AddButton(
                    x, y, w, h, text,
                    id ? id->c_str() : nullptr,
                    fontName.value_or(""),
                    fontScale.value_or(1.0f),
                    rounding.value_or(0.0f),
                    useColor.value_or(false),
                    norm(r.value_or(0.2f)),
                    norm(g.value_or(0.55f)),
                    norm(b.value_or(0.8f)),
                    a.value_or(1.0f)
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

        auto AddCheckboxStyled = [](int x, int y, float w, float h,
            const char* text, const std::string& id,
            sol::optional<bool> defaultValue,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale,
            sol::optional<sol::table> style)
            {
                auto norm = [](float v) -> float { return (v > 1.0f) ? (v / 255.0f) : v; };

                float sizePx = 0.0f;
                float rounding = -1.0f;
                bool useColors = false;

                ImVec4 onBg(0, 0, 0, 0);
                ImVec4 offBg(0, 0, 0, 0);
                ImVec4 check(0, 0, 0, 0);

                if (style)
                {
                    sol::table t = style.value();

                    if (sol::optional<float> v = t["size"])     sizePx = *v;
                    if (sol::optional<float> v = t["rounding"]) rounding = *v;

                    auto readColor = [&](const char* key, ImVec4& out) -> bool {
                        sol::object o = t[key];
                        if (!o.valid() || !o.is<sol::table>()) return false;
                        sol::table c = o.as<sol::table>();
                        float r = norm(c.get_or(1, 0.0f));
                        float g = norm(c.get_or(2, 0.0f));
                        float b = norm(c.get_or(3, 0.0f));
                        float a = c.get_or(4, 1.0f);
                        a = (a > 1.0f) ? (a / 255.0f) : a;
                        if (a < 0.0f) a = 0.0f;
                        if (a > 1.0f) a = 1.0f;
                        out = ImVec4(r, g, b, a);
                        return true;
                        };

                    bool any = false;
                    any = readColor("on", onBg) || any;
                    any = readColor("off", offBg) || any;
                    any = readColor("check", check) || any;
                    useColors = any;
                }

                Application::Get().layerUI->m_UIManager->AddCheckboxStyled(
                    x, y, w, h,
                    text, id.c_str(),
                    defaultValue.value_or(false),
                    fontName.value_or(""),
                    fontScale.value_or(1.0f),
                    sizePx, rounding, useColors,
                    onBg, offBg, check
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
            sol::optional<float> fontScale, sol::optional<std::string> format)
            {
                Application::Get().layerUI->m_UIManager->AddSlider(
                    x, y, width,
                    label, id.c_str(),
                    min, max,
                    defaultValue.value_or(0.0f),
                    fontName.value_or(""),
                    fontScale.value_or(1.0f),
                    format.value_or("")
                );
            };

        auto AddSliderStyled = [](int x, int y, float width,
            const char* label, const std::string& id,
            float min, float max,
            sol::optional<float> defaultValue,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale,
            sol::optional<std::string> format,
            sol::optional<sol::table> style)
            {
                auto norm = [](float v) -> float { return (v > 1.0f) ? (v / 255.0f) : v; };

                // Defaults = �gno override�h
                float heightPx = 0.0f;
                float rounding = -1.0f;
                float grabSize = 0.0f;
                bool useColors = false;

                ImVec4 track(0, 0, 0, 0);
                ImVec4 grab(0, 0, 0, 0);

                if (style)
                {
                    sol::table t = style.value();

                    if (sol::optional<float> v = t["height"])    heightPx = *v;
                    if (sol::optional<float> v = t["rounding"])  rounding = *v;
                    if (sol::optional<float> v = t["grab_size"]) grabSize = *v;

                    // track = {r,g,b,a} and grab = {r,g,b,a} (0..1 or 0..255)
                    auto readColor = [&](const char* key, ImVec4& out) -> bool {
                        sol::object o = t[key];
                        if (!o.valid() || !o.is<sol::table>()) return false;
                        sol::table c = o.as<sol::table>();

                        // support [1..4]
                        float r = c.get_or(1, 0.0f);
                        float g = c.get_or(2, 0.0f);
                        float b = c.get_or(3, 0.0f);
                        float a = c.get_or(4, 1.0f);

                        out = ImVec4(norm(r), norm(g), norm(b), norm(a));
                        return true;
                        };

                    bool hasTrack = readColor("track", track);
                    bool hasGrab = readColor("grab", grab);
                    useColors = hasTrack || hasGrab;
                }

                Application::Get().layerUI->m_UIManager->AddSliderStyled(
                    x, y, width,
                    label, id.c_str(),
                    min, max,
                    defaultValue.value_or(0.0f),
                    fontName.value_or(""),
                    fontScale.value_or(1.0f),
                    format.value_or(""),
                    heightPx, rounding, grabSize,
                    useColors, track, grab
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

        auto DrawProgressBar = [](int x, int y, float xSize, float ySize, float maxValue, float currentValue, int colorId) {
            Application::Get().layerUI->m_UIManager->AddProgressBarValue(
                x, y, xSize, ySize,
                maxValue, currentValue, colorId
            );
            };

        auto DrawProgressBarStyled = [](int x, int y, float xSize, float ySize,
            float maxValue, float currentValue, int colorId,
            sol::optional<sol::table> style,
            sol::optional<std::string> overlayText)
            {
                auto norm = [](float v) -> float { return (v > 1.0f) ? (v / 255.0f) : v; };

                float rounding = -1.0f;
                float borderSize = -1.0f;
                bool useColors = false;
                ImVec4 bg = ImVec4(0, 0, 0, 0);
                ImVec4 fill = ImVec4(0, 0, 0, 0);
                ImVec4 border = ImVec4(0, 0, 0, 0);


                float capMax = -1.0f;
                ImVec4 capFill = ImVec4(1.0f, 0.82f, 0.24f, 0.86f);
                if (style)
                {
                    sol::table t = style.value();

                    if (sol::optional<float> v = t["rounding"]) rounding = *v;
                    if (sol::optional<float> v = t["border_size"]) borderSize = *v;


                    auto readColor = [&](const char* key, ImVec4& out) -> bool
                        {
                            sol::object o = t[key];
                            if (!o.valid() || !o.is<sol::table>()) return false;
                            sol::table c = o.as<sol::table>();
                            float r = norm(c.get_or(1, 0.0f));
                            float g = norm(c.get_or(2, 0.0f));
                            float b = norm(c.get_or(3, 0.0f));
                            float a = norm(c.get_or(4, 1.0f));
                            out = ImVec4(r, g, b, a);
                            return true;
                        };

                    bool any = false;
                    any = readColor("bg", bg) || any;
                    any = readColor("fill", fill) || any;
                    any = readColor("border", border) || any;

                    if (sol::optional<float> v = t["cap_max"]) capMax = *v;
                    bool hasCapFill = readColor("cap_fill", capFill);

                    useColors = any || hasCapFill;
                }

                Application::Get().layerUI->m_UIManager->AddProgressBarValueStyled(
                    x, y, xSize, ySize,
                    maxValue, currentValue, colorId,
                    rounding, borderSize,
                    useColors, bg, fill, border,
                    overlayText.value_or(""),
                    "", 1.0f,
                    capMax, capFill
                );
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

        auto GetInputTextLive = [](const std::string& id) {
            return Application::Get().layerUI->m_UIManager->GetLiveText(id);
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

        auto AddDropdownStyled = [](int x, int y, float w, float h, const char* label,
            const std::string& id, sol::table options,
            sol::optional<int> defaultIndex,
            sol::optional<std::string> fontName,
            sol::optional<float> fontScale,
            sol::optional<sol::table> style)
            {
                std::vector<std::string> opts;
                for (auto& kv : options)
                {
                    sol::object v = kv.second;
                    if (v.is<std::string>()) opts.push_back(v.as<std::string>());
                }

                auto norm = [](float v) -> float { return (v > 1.0f) ? (v / 255.0f) : v; };

                float heightPx = 0.0f;
                float rounding = -1.0f;
                float popupRounding = -1.0f;
                float borderSize = -1.0f;

                bool hasFrame = false;         ImVec4 frame(0, 0, 0, 0);
                bool hasFrameHovered = false;  ImVec4 frameHovered(0, 0, 0, 0);
                bool hasFrameActive = false;   ImVec4 frameActive(0, 0, 0, 0);

                bool hasPopupBg = false;       ImVec4 popupBg(0, 0, 0, 0);
                bool hasBorder = false;        ImVec4 borderCol(0, 0, 0, 0);

                bool hasItem = false;          ImVec4 item(0, 0, 0, 0);
                bool hasItemHovered = false;   ImVec4 itemHovered(0, 0, 0, 0);
                bool hasItemActive = false;    ImVec4 itemActive(0, 0, 0, 0);

                bool hasText = false;          ImVec4 textCol(0, 0, 0, 0);

                if (style)
                {
                    sol::table t = style.value();

                    if (t["height"].valid())          heightPx = t["height"];
                    if (t["rounding"].valid())        rounding = t["rounding"];
                    if (t["popup_rounding"].valid())  popupRounding = t["popup_rounding"];
                    if (t["border_size"].valid())     borderSize = t["border_size"];

                    auto readColor = [&](const char* key, bool& has, ImVec4& out)
                        {
                            sol::object obj = t[key];
                            if (!obj.valid() || !obj.is<sol::table>()) return;
                            sol::table c = obj.as<sol::table>();
                            out = ImVec4(
                                norm(c.get_or(1, 0.0f)),
                                norm(c.get_or(2, 0.0f)),
                                norm(c.get_or(3, 0.0f)),
                                norm(c.get_or(4, 1.0f))
                            );
                            has = true;
                        };

                    readColor("frame", hasFrame, frame);
                    readColor("frame_hover", hasFrameHovered, frameHovered);
                    readColor("frame_active", hasFrameActive, frameActive);

                    readColor("popup_bg", hasPopupBg, popupBg);
                    readColor("border", hasBorder, borderCol);

                    // Selectable colors inside the dropdown list
                    readColor("item", hasItem, item);
                    readColor("item_hover", hasItemHovered, itemHovered);
                    readColor("item_active", hasItemActive, itemActive);

                    readColor("text", hasText, textCol);
                }

                Application::Get().layerUI->m_UIManager->AddDropdownStyled(
                    x, y, w, h, label,
                    id.c_str(), opts,
                    defaultIndex.value_or(0),
                    fontName.value_or(""),
                    fontScale.value_or(1.0f),
                    heightPx,
                    rounding, popupRounding, borderSize,
                    hasFrame, frame,
                    hasFrameHovered, frameHovered,
                    hasFrameActive, frameActive,
                    hasPopupBg, popupBg,
                    hasBorder, borderCol,
                    hasItem, item,
                    hasItemHovered, itemHovered,
                    hasItemActive, itemActive,
                    hasText, textCol
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
            sol::optional<int> flags,
            sol::optional<bool> hasBg,
            sol::optional<float> alpha,
            sol::optional<float> rounding,
            sol::optional<int> r,
            sol::optional<int> g,
            sol::optional<int> b,
            sol::optional<float> borderSize,
            sol::optional<bool> autoBorder,
            sol::optional<float> borderDarken,
            sol::optional<int> borderR,
            sol::optional<int> borderG,
            sol::optional<int> borderB,
            sol::optional<float> borderA)
            {
                auto norm255i = [](int v) -> float {
                    if (v < 0) v = 0;
                    if (v > 255) v = 255;
                    return v / 255.0f;
                    };

                auto normA = [](float v) -> float {
                    // allow 0..1 or 0..255
                    if (v > 1.0f) v = v / 255.0f;
                    if (v < 0.0f) v = 0.0f;
                    if (v > 1.0f) v = 1.0f;
                    return v;
                    };

                ImVec4 bc(
                    norm255i(borderR.value_or(0)),
                    norm255i(borderG.value_or(0)),
                    norm255i(borderB.value_or(0)),
                    normA(borderA.value_or(1.0f))
                );

                Application::Get().layerUI->m_UIManager->BeginChild(
                    x, y, w, h,
                    id.c_str(),
                    border.value_or(false),
                    (ImGuiWindowFlags)flags.value_or(0),
                    hasBg.value_or(false),
                    alpha.value_or(0.45f),
                    rounding.value_or(6.0f),
                    r.value_or(0),
                    g.value_or(0),
                    b.value_or(0),
                    borderSize.value_or(1.0f),
                    autoBorder.value_or(true),
                    borderDarken.value_or(0.85f),
                    bc
                );
            };


        auto EndChild = []()
            {
                Application::Get().layerUI->m_UIManager->EndChild();
            };

        lua["UI"] = lua.create_table_with(
            "add_label", AddLabel,
            "add_centered_label", AddCenteredLabel,
            "add_label_colored", AddLabelColored,
            "add_centered_label_colored", AddCenteredLabelColored,
            "add_button", AddButton,
            "add_checkbox", AddCheckbox,
            "add_checkbox_styled", AddCheckboxStyled,
            "add_slider", AddSlider,
            "add_slider_styled", AddSliderStyled,
            "draw_progress_bar", DrawProgressBar,
            "draw_progress_bar_styled", DrawProgressBarStyled,
            "add_input_text", AddInputText,
            "add_panel", AddPanel,
            "add_radio", AddRadio,
            "add_dropdown", AddDropdown,
            "add_dropdown_styled", AddDropdownStyled,
            "add_color_picker", AddColorPicker,

            "was_button_pressed", WasButtonPressed,
            "get_checkbox", GetCheckbox,
            "was_checkbox_changed", WasCheckboxChanged,
            "get_slider", GetSlider,
            "was_slider_changed", WasSliderChanged,
            "set_slider", SetSlider,
            "get_input_text", GetInputText,
            "was_input_committed", WasInputCommitted,
            "get_input_text_live", GetInputTextLive,
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