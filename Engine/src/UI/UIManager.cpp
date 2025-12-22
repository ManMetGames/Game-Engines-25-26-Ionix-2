#include "UIManager.h"
#include <iostream>
#include <imgui.h>

namespace IonixEngine
{

	void UIManager::AddChildToPanel(UIElement* element)
	{
		elements.push_back(element);
	}

	void UIManager::AddLabel(int x, int y, float xSize, float ySize, const char* text, const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement;
		element->type = UIType::Label;
		element->xPos = x;
		element->yPos = y;
		element->xSize = xSize;
		element->ySize = ySize;
		element->ownedText = (text ? text : "");
		element->text = element->ownedText.c_str();
		element->fontName = fontName;
		element->fontScale = fontScale;
		AddChildToPanel(element);
	}

	void UIManager::AddCenteredLabel(float centerX, float y, const char* text, const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Label;
		element->centerAligned = true;
		element->centerX = centerX;
		element->yPos = (int)y;

		element->fontName = fontName;
		element->fontScale = fontScale;

		element->ownedText = (text ? text : "");
		element->text = element->ownedText.c_str();

		element->fontName = fontName;
		AddChildToPanel(element);
	}

	void UIManager::AddLabelColored(int x, int y, float xSize, float ySize, const char* text,
		float r, float g, float b, float a,
		const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement;
		element->type = UIType::Label;
		element->xPos = x;
		element->yPos = y;
		element->xSize = xSize;
		element->ySize = ySize;

		element->ownedText = (text ? text : "");
		element->text = element->ownedText.c_str();

		element->fontName = fontName;
		element->fontScale = fontScale;

		element->hasTextColor = true;
		element->textColor = ImVec4(r, g, b, a);

		AddChildToPanel(element);
	}

	void UIManager::AddCenteredLabelColored(float centerX, float y, const char* text,
		float r, float g, float b, float a,
		const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Label;
		element->centerAligned = true;
		element->centerX = centerX;
		element->yPos = (int)y;

		element->fontName = fontName;
		element->fontScale = fontScale;

		element->ownedText = (text ? text : "");
		element->text = element->ownedText.c_str();

		element->hasTextColor = true;
		element->textColor = ImVec4(r, g, b, a);

		AddChildToPanel(element);
	}

	void UIManager::AddButton(int x, int y, float xSize, float ySize, const char* text, const char* id, const std::string& fontName, float fontScale, float rounding, bool useColor,
		float r, float g, float b, float a)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Button;
		element->xPos = x;
		element->yPos = y;
		element->xSize = xSize;
		element->ySize = ySize;

		element->fontName = fontName;
		element->fontScale = fontScale;

		element->buttonCustomStyle = (rounding > 0 || useColor);
		element->buttonRounding = rounding;

		if (useColor)
			element->buttonColor = ImVec4(r, g, b, a);
		else
			element->buttonColor = ImVec4(0, 0, 0, 0);
		element->ownedText = (text ? text : "");
		element->text = element->ownedText.c_str();

		element->widgetId = (id && id[0]) ? id : element->ownedText; // fallback

		AddChildToPanel(element);
	}

	bool UIManager::WasButtonPressed(const std::string& id)
	{
		auto it = m_buttonPressed.find(id);
		if (it == m_buttonPressed.end()) return false;

		bool pressed = it->second;
		it->second = false; // consume
		return pressed;
	}

	void UIManager::AddCheckbox(int x, int y, float xSize, float ySize, const char* text, const char* id, bool defaultValue, const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Checkbox;
		element->xPos = x;
		element->yPos = y;
		element->xSize = xSize;
		element->ySize = ySize;

		element->ownedText = (text ? text : "");
		element->text = element->ownedText.c_str();

		element->fontName = fontName;
		element->fontScale = fontScale;

		element->widgetId = (id && id[0]) ? id : element->ownedText;
		element->defaultValue = defaultValue;

		// only set default once
		if (m_checkboxValues.find(element->widgetId) == m_checkboxValues.end())
			m_checkboxValues[element->widgetId] = defaultValue;

		AddChildToPanel(element);
	}


	void UIManager::AddCheckboxStyled(int x, int y, float xSize, float ySize,
		const char* text, const char* id, bool defaultValue,
		const std::string& fontName, float fontScale,
		float sizePx, float rounding, bool useColors,
		ImVec4 onBg, ImVec4 offBg, ImVec4 check)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Checkbox;
		element->xPos = x;
		element->yPos = y;
		element->xSize = xSize;
		element->ySize = ySize;

		element->ownedText = (text ? text : "");
		element->text = element->ownedText.c_str();

		element->fontName = fontName;
		element->fontScale = fontScale;

		element->widgetId = (id && id[0]) ? id : element->ownedText;
		element->defaultValue = defaultValue;

		// only set default once
		if (m_checkboxValues.find(element->widgetId) == m_checkboxValues.end())
			m_checkboxValues[element->widgetId] = defaultValue;

		element->checkboxCustomStyle = true;
		element->checkboxSizePx = sizePx;
		element->checkboxRounding = rounding;
		element->checkboxUseColors = useColors;
		element->checkboxOnBg = onBg;
		element->checkboxOffBg = offBg;
		element->checkboxCheck = check;

		AddChildToPanel(element);
	}

	bool UIManager::GetCheckbox(const std::string& id) const
	{
		auto it = m_checkboxValues.find(id);
		return (it != m_checkboxValues.end()) ? it->second : false;
	}

	bool UIManager::WasCheckboxChanged(const std::string& id)
	{
		auto it = m_checkboxChanged.find(id);
		if (it == m_checkboxChanged.end()) return false;

		bool changed = it->second;
		it->second = false; // consume
		return changed;
	}


	void UIManager::AddSlider(int x, int y, float width,
		const char* label, const char* id,
		float min, float max, float defaultValue, const std::string& fontName, float fontScale, const std::string& format)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Slider;
		element->xPos = x;
		element->yPos = y;
		element->xSize = width;          // use xSize as width
		element->ySize = 0.0f;

		element->ownedText = (label ? label : "");
		element->text = element->ownedText.c_str();

		element->fontName = fontName;
		element->fontScale = fontScale;

		element->widgetId = (id && id[0]) ? id : element->ownedText;
		element->sliderMin = min;
		element->slidermax = max;
		element->sliderFormat = format;

		// init only once
		if (m_sliderValues.find(element->widgetId) == m_sliderValues.end())
			m_sliderValues[element->widgetId] = defaultValue;

		AddChildToPanel(element);
	}

	void UIManager::AddSliderStyled(int x, int y, float width,
		const char* label, const char* id,
		float min, float max, float defaultValue,
		const std::string& fontName, float fontScale,
		const std::string& format,
		float heightPx, float rounding, float grabSize,
		bool useColors, ImVec4 track, ImVec4 grab)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Slider;
		element->xPos = x;
		element->yPos = y;
		element->xSize = width;          // use xSize as width
		element->ySize = 0.0f;

		element->ownedText = (label ? label : "");
		element->text = element->ownedText.c_str();

		element->fontName = fontName;
		element->fontScale = fontScale;

		element->widgetId = (id && id[0]) ? id : element->ownedText;
		element->sliderMin = min;
		element->slidermax = max;
		element->sliderFormat = format;

		element->sliderCustomStyle = true;
		element->sliderHeightPx = heightPx;
		element->sliderRounding = rounding;
		element->sliderGrabMinSize = grabSize;
		element->sliderUseColors = useColors;
		element->sliderTrack = track;
		element->sliderGrab = grab;

		// init only once
		if (m_sliderValues.find(element->widgetId) == m_sliderValues.end())
			m_sliderValues[element->widgetId] = defaultValue;

		AddChildToPanel(element);
	}

	float UIManager::GetSlider(const std::string& id) const
	{
		auto it = m_sliderValues.find(id);
		return (it != m_sliderValues.end()) ? it->second : 0.0f;
	}

	bool UIManager::WasSliderChanged(const std::string& id)
	{
		auto it = m_sliderChanged.find(id);
		if (it == m_sliderChanged.end()) return false;

		bool changed = it->second;
		it->second = false; // consume like your buttons/checkboxes
		return changed;
	}

	void UIManager::SetSlider(const std::string& id, float v)
	{
		m_sliderValues[id] = v;
	}

	void UIManager::AddInputText(int xPos, int yPos, float width, const char* label, const char* id, size_t maxLen, const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement;
		element->type = UIType::InputText;
		element->xPos = xPos;
		element->yPos = yPos;
		element->width = width;
		element->ownedText = (label ? label : "");
		element->text = element->ownedText.c_str();
		element->inputId = id;

		element->fontName = fontName;
		element->fontScale = fontScale;

		auto& buf = m_inputBuffers[element->inputId];
		if (buf.empty())
			buf.assign(maxLen, '\0');   // persistent storage
		element->inputBuffer = buf.data();
		element->inputBufferSize = 16;
		element->inputBufferSize = buf.size();
		AddChildToPanel(element);
	}

	// - To fetch inputted text from AddInputText
	std::string IonixEngine::UIManager::GetCommittedText(const std::string& id) const
	{
		auto it = m_committedText.find(id);
		return (it == m_committedText.end()) ? "" : it->second;
	}

	bool UIManager::WasInputCommitted(const std::string& id) const
	{
		auto it = m_inputCommittedThisFrame.find(id);
		return it != m_inputCommittedThisFrame.end() && it->second;
	}

	std::string IonixEngine::UIManager::GetLiveText(const std::string& id) const
	{
		auto it = m_inputBuffers.find(id);
		if (it == m_inputBuffers.end() || it->second.empty())
			return "";

		// buffer is kept null-terminated by ImGui
		return std::string(it->second.data());
	}

	void UIManager::ClearInput(const std::string& id)
	{
		// clear committed text
		m_committedText[id].clear();

		// clear live buffer
		auto it = m_inputBuffers.find(id);
		if (it != m_inputBuffers.end() && !it->second.empty())
			it->second[0] = '\0';
	}

	void UIManager::AddRadioToggle(int x, int y, float xSize, float ySize, const char* label,
		const char* groupId, int value, int defaultValue, bool sameline,
		const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::RadioButton;
		element->xPos = x; element->yPos = y;
		element->xSize = xSize; element->ySize = ySize;
		element->ownedText = (label ? label : "");
		element->text = element->ownedText.c_str();

		element->groupId = (groupId ? groupId : "");
		element->radioValue = value;
		element->radioDefaultValue = defaultValue;
		element->sameline = sameline;

		element->fontName = fontName;
		element->fontScale = fontScale;

		if (!element->groupId.empty() && m_radioGroupValue.find(element->groupId) == m_radioGroupValue.end())
			m_radioGroupValue[element->groupId] = defaultValue;

		AddChildToPanel(element);
	}


	void UIManager::AddColorPicker(int x, int y, float xSize, float ySize, const char* label,
		const char* id, ImVec4 defaultColor,
		const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::ColorPicker;
		element->xPos = x; element->yPos = y;
		element->xSize = xSize; element->ySize = ySize;
		element->ownedText = (label ? label : "");
		element->text = element->ownedText.c_str();

		element->widgetId = (id && id[0]) ? id : element->ownedText;
		element->colorDefault = defaultColor;

		element->fontName = fontName;
		element->fontScale = fontScale;

		if (m_colorValues.find(element->widgetId) == m_colorValues.end())
			m_colorValues[element->widgetId] = defaultColor;

		AddChildToPanel(element);
	}

	void UIManager::AddDropdown(int x, int y, float xSize, float ySize, const char* label,
		const char* id, const std::vector<std::string>& options, int defaultIndex,
		const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Dropdown;
		element->xPos = x; element->yPos = y;
		element->xSize = xSize; element->ySize = ySize;

		element->ownedText = (label ? label : "");
		element->text = element->ownedText.c_str();

		element->widgetId = (id && id[0]) ? id : element->ownedText;
		element->dropdownOptions = options;

		element->fontName = fontName;
		element->fontScale = fontScale;

		if (m_dropdownIndex.find(element->widgetId) == m_dropdownIndex.end())
			m_dropdownIndex[element->widgetId] = defaultIndex;

		AddChildToPanel(element);
	}


	void UIManager::AddDropdownStyled(int x, int y, float xSize, float ySize, const char* label,
		const char* id, const std::vector<std::string>& options, int defaultIndex,
		const std::string& fontName, float fontScale,
		float heightPx,
		float rounding, float popupRounding, float borderSize,
		bool hasFrame, ImVec4 frame,
		bool hasFrameHovered, ImVec4 frameHovered,
		bool hasFrameActive, ImVec4 frameActive,
		bool hasPopupBg, ImVec4 popupBg,
		bool hasBorder, ImVec4 borderCol,
		bool hasItem, ImVec4 item,
		bool hasItemHovered, ImVec4 itemHovered,
		bool hasItemActive, ImVec4 itemActive,
		bool hasText, ImVec4 textCol)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Dropdown;
		element->xPos = x; element->yPos = y;
		element->xSize = xSize; element->ySize = (heightPx > 0.0f) ? heightPx : ySize;

		element->ownedText = (label ? label : "");
		element->text = element->ownedText.c_str();

		element->widgetId = (id && id[0]) ? id : element->ownedText;
		element->dropdownOptions = options;

		element->fontName = fontName;
		element->fontScale = fontScale;

		// Init dropdown state once
		if (m_dropdownIndex.find(element->widgetId) == m_dropdownIndex.end())
			m_dropdownIndex[element->widgetId] = defaultIndex;

		// Style
		element->dropdownCustomStyle = true;
		element->dropdownRounding = rounding;
		element->dropdownPopupRounding = popupRounding;
		element->dropdownBorderSize = borderSize;

		element->ddHasFrame = hasFrame;
		element->ddFrame = frame;
		element->ddHasFrameHovered = hasFrameHovered;
		element->ddFrameHovered = frameHovered;
		element->ddHasFrameActive = hasFrameActive;
		element->ddFrameActive = frameActive;

		element->ddHasPopupBg = hasPopupBg;
		element->ddPopupBg = popupBg;

		element->ddHasBorder = hasBorder;
		element->ddBorder = borderCol;

		element->ddHasItem = hasItem;
		element->ddItem = item;
		element->ddHasItemHovered = hasItemHovered;
		element->ddItemHovered = itemHovered;
		element->ddHasItemActive = hasItemActive;
		element->ddItemActive = itemActive;

		element->ddHasText = hasText;
		element->ddText = textCol;

		AddChildToPanel(element);
	}


	void UIManager::AddProgressBar(int x, int y, float xSize, float ySize, float maxvalue, float* currentvalue, float incrementamount, const std::string& fontName)
	{
		UIElement* element = new UIElement;
		element->type = UIType::ProgressBar;
		element->xPos = x;
		element->yPos = y;
		element->xSize = xSize;
		element->ySize = ySize;
		element->maxValue = maxvalue;
		element->currentValue = currentvalue;
		element->incrementAmount = incrementamount;
		element->fontName = fontName;
		AddChildToPanel(element);
	}


	void UIManager::AddProgressBarValue(int x, int y, float xSize, float ySize,
		float maxValue, float currentValue, int colorId,
		const std::string& fontName, float fontScale)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::ProgressBar;
		element->xPos = x;
		element->yPos = y;
		element->xSize = xSize;
		element->ySize = ySize;

		element->maxValue = maxValue;
		element->currentValue = nullptr;          // value-mode
		element->incrementAmount = 0.0f;

		element->progressCurrentValue = currentValue;
		element->progressColorId = colorId;

		element->fontName = fontName;
		element->fontScale = fontScale;

		AddChildToPanel(element);
	}

	void UIManager::AddProgressBarValueStyled(int x, int y, float xSize, float ySize,
		float maxValue, float currentValue, int colorId,
		float rounding, float borderSize,
		bool useColors, ImVec4 bg, ImVec4 fill, ImVec4 border,
		const std::string& overlayText,
		const std::string& fontName, float fontScale,
		float capMax, ImVec4 capFill)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::ProgressBar;
		element->xPos = x;
		element->yPos = y;
		element->xSize = xSize;
		element->ySize = ySize;

		element->maxValue = maxValue;
		element->currentValue = nullptr;          // value-mode
		element->incrementAmount = 0.0f;

		element->progressCurrentValue = currentValue;
		element->progressColorId = colorId;

		element->progressCustomStyle = true;
		element->progressRounding = rounding;
		element->progressBorderSize = borderSize;
		element->progressUseColors = useColors;
		element->progressBg = bg;
		element->progressFill = fill;
		element->progressBorder = border;
		element->progressOverlayText = overlayText;


		element->progressCapMax = capMax;
		element->progressCapFill = capFill;
		element->fontName = fontName;
		element->fontScale = fontScale;

		AddChildToPanel(element);
	}

	void IonixEngine::UIManager::AddPanel(int x, int y, float w, float h,
		float alpha, float rounding,
		int r, int g, int b)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::Panel;
		element->xPos = x;
		element->yPos = y;
		element->xSize = w;
		element->ySize = h;
		element->panelAlpha = alpha;
		element->panelRounding = rounding;
		element->panelR = r;
		element->panelG = g;
		element->panelB = b;
		AddChildToPanel(element);
	}

	int UIManager::GetRadio(const std::string& groupId) const
	{
		auto it = m_radioGroupValue.find(groupId);
		return (it != m_radioGroupValue.end()) ? it->second : 0;
	}

	bool UIManager::WasRadioChanged(const std::string& groupId)
	{
		auto it = m_radioChanged.find(groupId);
		if (it == m_radioChanged.end()) return false;
		bool v = it->second;
		it->second = false;
		return v;
	}

	int UIManager::GetDropdownIndex(const std::string& id) const
	{
		auto it = m_dropdownIndex.find(id);
		return (it != m_dropdownIndex.end()) ? it->second : 0;
	}

	bool UIManager::WasDropdownChanged(const std::string& id)
	{
		auto it = m_dropdownChanged.find(id);
		if (it == m_dropdownChanged.end()) return false;
		bool v = it->second;
		it->second = false;
		return v;
	}

	ImVec4 UIManager::GetColor(const std::string& id) const
	{
		auto it = m_colorValues.find(id);
		return (it != m_colorValues.end()) ? it->second : ImVec4(1, 1, 1, 1);
	}

	bool UIManager::WasColorChanged(const std::string& id)
	{
		auto it = m_colorChanged.find(id);
		if (it == m_colorChanged.end()) return false;
		bool v = it->second;
		it->second = false;
		return v;
	}

	void UIManager::BeginChild(int x, int y, float w, float h, const char* id,
		bool border, ImGuiWindowFlags flags,
		bool hasBg, float alpha, float rounding,
		int r, int g, int b, float borderSize,
		bool autoBorder, float borderDarken,
		ImVec4 borderColor)
	{
		UIElement* element = new UIElement{};
		element->type = UIType::BeginChild;
		element->xPos = x; element->yPos = y;
		element->xSize = w; element->ySize = h;

		element->childId = (id && id[0]) ? id : "Child";
		element->childBorder = border;
		element->childFlags = flags;

		element->childHasBg = hasBg;
		element->childBgAlpha = alpha;
		element->childBgRounding = rounding;
		element->childBgR = r; element->childBgG = g; element->childBgB = b;

		element->childBorderSize = borderSize;
		element->childAutoBorderColor = autoBorder;
		element->childBorderDarken = borderDarken;
		element->childBorderColor = borderColor;

		AddChildToPanel(element);
	}


	void UIManager::EndChild()
	{
		UIElement* element = new UIElement{};
		element->type = UIType::EndChild;
		AddChildToPanel(element);
	}

	void UIManager::ClearElements()
	{
		for (auto* e : elements)
			delete e;
		elements.clear();
	}

	void UIManager::RenderElement(UIElement* element)
	{
		ImFont* font = nullptr;
		if (!element->fontName.empty())
			font = fontLoader.GetFont(element->fontName);

		if (font) ImGui::PushFont(font);

		if (element->fontScale != 1.0f)
			ImGui::SetWindowFontScale(element->fontScale);

		switch (element->type)
		{
		case UIType::Label:
		{
			float x = (float)element->xPos;

			if (element->centerAligned)
			{
				// This runs during rendering, with the correct font pushed already
				float w = ImGui::CalcTextSize(element->text).x;
				x = element->centerX - (w * 0.5f);
			}

			bool pushedTextColor = false;
			if (element->hasTextColor)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, element->textColor);
				pushedTextColor = true;
			}

			m_ui->DrawLabel(element->text, element->xSize, element->ySize, (int)x, element->yPos);

			if (pushedTextColor) ImGui::PopStyleColor();
			break;
		}
		case UIType::Button:
		{
			// Make ImGui id unique: "Visible##id"
			std::string imguiLabel = element->ownedText + "##" + element->widgetId;

			int pushedColors = 0;
			bool pushedRounding = false;

			if (element->buttonCustomStyle)
			{
				if (element->buttonRounding > 0.0f) {
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, element->buttonRounding);
					pushedRounding = true;
				}

				if (element->buttonColor.w > 0.0f) {
					ImVec4 base = element->buttonColor;
					ImVec4 hover = ImVec4(std::min(base.x + 0.06f, 1.0f), std::min(base.y + 0.06f, 1.0f), std::min(base.z + 0.06f, 1.0f), base.w);
					ImVec4 active = ImVec4(std::max(base.x - 0.06f, 0.0f), std::max(base.y - 0.06f, 0.0f), std::max(base.z - 0.06f, 0.0f), base.w);

					ImGui::PushStyleColor(ImGuiCol_Button, base);        pushedColors++;
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover); pushedColors++;
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, active); pushedColors++;
				}
			}

			bool pressed = m_ui->DrawButton(
				const_cast<char*>(imguiLabel.c_str()),
				(int)element->xSize, (int)element->ySize,
				element->xPos, element->yPos
			);

			if (pushedColors) ImGui::PopStyleColor(pushedColors);
			if (pushedRounding) ImGui::PopStyleVar();

			if (pressed)
				m_buttonPressed[element->widgetId] = true;

			break;
		}

		case UIType::Checkbox:
		{
			if (element->widgetId.empty())
				break;

			bool& v = m_checkboxValues[element->widgetId];

			// Styled checkbox (drawn manually so Lua can control size/rounding/colors)
			if (element->checkboxCustomStyle)
			{
				const float x = (float)element->xPos;
				const float y = (float)element->yPos;

				ImGui::SetCursorPos(ImVec2(x, y));

				const float size = (element->checkboxSizePx > 0.0f)
					? element->checkboxSizePx
					: ImGui::GetFrameHeight();

				std::string btnId = "##" + element->widgetId;
				ImVec2 p0 = ImGui::GetCursorScreenPos();

				const bool clicked = ImGui::InvisibleButton(btnId.c_str(), ImVec2(size, size));
				const bool hovered = ImGui::IsItemHovered();

				if (clicked)
				{
					v = !v;
					m_checkboxChanged[element->widgetId] = true;
				}

				ImDrawList* dl = ImGui::GetWindowDrawList();

				const float rounding = (element->checkboxRounding >= 0.0f)
					? element->checkboxRounding
					: ImGui::GetStyle().FrameRounding;

				ImVec4 bg = v ? element->checkboxOnBg : element->checkboxOffBg;
				if (!element->checkboxUseColors)
				{
					bg = ImGui::GetStyleColorVec4(v ? ImGuiCol_FrameBgActive : ImGuiCol_FrameBg);
				}
				else if (hovered)
				{
					// subtle brighten on hover
					bg.x = std::min(bg.x + 0.05f, 1.0f);
					bg.y = std::min(bg.y + 0.05f, 1.0f);
					bg.z = std::min(bg.z + 0.05f, 1.0f);
				}

				ImU32 colBg = ImGui::ColorConvertFloat4ToU32(bg);
				ImU32 colBorder = ImGui::GetColorU32(ImGuiCol_Border);

				ImVec2 p1(p0.x + size, p0.y + size);
				dl->AddRectFilled(p0, p1, colBg, rounding);
				dl->AddRect(p0, p1, colBorder, rounding);

				if (v)
				{
					ImVec4 ck = element->checkboxUseColors ? element->checkboxCheck : ImGui::GetStyleColorVec4(ImGuiCol_CheckMark);
					ImU32 colCk = ImGui::ColorConvertFloat4ToU32(ck);

					const float pad = size * 0.22f;
					const float thick = std::max(2.0f, size * 0.12f);

					ImVec2 a(p0.x + pad, p0.y + size * 0.55f);
					ImVec2 b(p0.x + size * 0.44f, p0.y + size - pad);
					ImVec2 c(p0.x + size - pad, p0.y + pad);

					dl->AddLine(a, b, colCk, thick);
					dl->AddLine(b, c, colCk, thick);
				}

				// Label (drawn separately so we can keep absolute positioning)
				if (!element->ownedText.empty())
				{
					ImGui::SetCursorPos(ImVec2(x + size + 10.0f, y + (size - ImGui::GetTextLineHeight()) * 0.5f));
					ImGui::TextUnformatted(element->ownedText.c_str());
				}

				break;
			}

			// Default ImGui checkbox
			std::string imguiLabel = element->ownedText + "##" + element->widgetId;

			bool changed = m_ui->DrawCheckbox(imguiLabel.c_str(), &v, element->xPos, element->yPos);

			if (changed)
				m_checkboxChanged[element->widgetId] = true;

			break;
		}
		case UIType::Slider:
		{
			if (element->widgetId.empty())
				break;

			float& v = m_sliderValues[element->widgetId];

			// "Visible label##id" trick
			std::string imguiLabel = element->ownedText + "##" + element->widgetId;

			// IMPORTANT: if format is empty, pass nullptr so ImGui uses default formatting
			const char* fmt = nullptr;
			if (!element->sliderFormat.empty())
				fmt = element->sliderFormat.c_str();

			int pushedVars = 0;
			int pushedColors = 0;

			// Only styled sliders push these overrides
			if (element->sliderCustomStyle)
			{
				// Thickness/height via frame padding Y
				if (element->sliderHeightPx > 0.0f)
				{
					// A simple approach: translate "heightPx" into extra padding.
					// Feel free to tweak this formula to taste.
					float padY = std::max(0.0f, (element->sliderHeightPx - ImGui::GetFontSize()) * 0.5f);
					ImGuiStyle& st = ImGui::GetStyle();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(st.FramePadding.x, padY));
					pushedVars++;
				}

				// Rounding (track + grab)
				if (element->sliderRounding >= 0.0f)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, element->sliderRounding);
					pushedVars++;

					float grabRound = (element->sliderGrabRounding >= 0.0f)
						? element->sliderGrabRounding
						: element->sliderRounding;

					ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, grabRound);
					pushedVars++;
				}

				// Grab size
				if (element->sliderGrabMinSize > 0.0f)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, element->sliderGrabMinSize);
					pushedVars++;
				}

				// Colors (track + grab)
				if (element->sliderUseColors)
				{
					// If you stored colors as ImVec4 already, just push them.
					// You can optionally derive hovered/active variants if you want.
					ImGui::PushStyleColor(ImGuiCol_FrameBg, element->sliderTrack); pushedColors++;
					ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, element->sliderTrack); pushedColors++;
					ImGui::PushStyleColor(ImGuiCol_FrameBgActive, element->sliderTrack); pushedColors++;

					ImGui::PushStyleColor(ImGuiCol_SliderGrab, element->sliderGrab); pushedColors++;
					ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, element->sliderGrab); pushedColors++;
				}
			}

			bool changed = m_ui->DrawSlider(
				imguiLabel.c_str(),
				&v,
				element->xSize,          // width
				element->xPos, element->yPos,
				element->sliderMin, element->slidermax,
				fmt
			);

			if (pushedColors) ImGui::PopStyleColor(pushedColors);
			if (pushedVars)   ImGui::PopStyleVar(pushedVars);

			if (changed)
				m_sliderChanged[element->widgetId] = true;

			break;
		}



		case UIType::InputText:
		{
			ImGui::PushID(element->inputId.c_str());

			bool pressedEnter = m_ui->InputText(
				element->text,
				element->xPos, element->yPos,
				element->width,
				element->inputBuffer, element->inputBufferSize,
				ImGuiInputTextFlags_EnterReturnsTrue
			);

			ImGui::PopID();

			if (pressedEnter)
			{
				m_committedText[element->inputId] = std::string(element->inputBuffer);
				m_inputCommittedThisFrame[element->inputId] = true;
				std::cout << "Entered name: " << m_committedText[element->inputId] << "\n";
			}
			break;
		}
		case UIType::RadioButton:
		{
			if (element->groupId.empty()) break;

			int& v = m_radioGroupValue[element->groupId];

			// Unique ID: Visible##group_value
			std::string imguiLabel = element->ownedText + "##" + element->groupId + "_" + std::to_string(element->radioValue);

			bool changed = m_ui->DrawRadioButton(element->xPos, element->yPos, imguiLabel.c_str(), &v, element->radioValue);
			if (changed)
				m_radioChanged[element->groupId] = true;

			break;
		}

		case UIType::ColorPicker:
		{
			if (element->widgetId.empty()) break;

			ImVec4& c = m_colorValues[element->widgetId];

			std::string imguiLabel = element->ownedText + "##" + element->widgetId;

			bool changed = m_ui->DrawColorPicker(element->xPos, element->yPos,
				element->xSize, element->ySize,
				imguiLabel.c_str(), (float*)&c);
			if (changed)
				m_colorChanged[element->widgetId] = true;

			break;
		}

		case UIType::ProgressBar:
		{
			if (element->currentValue)
			{
				m_ui->ProgressBar(
					element->xPos, element->yPos,
					element->xSize, element->ySize,
					element->maxValue,
					*element->currentValue,
					element->incrementAmount
				);
			}
			else
			{
				if (element->progressCustomStyle)
				{
					if (element->progressCapMax > 0.0f && element->progressCapMax < element->maxValue && element->progressCapFill.w > 0.0f)
					{
						m_ui->DrawProgressBarStyledCapped(
							element->xPos, element->yPos,
							element->xSize, element->ySize,
							element->maxValue,
							element->progressCurrentValue,
							element->progressColorId,
							element->progressRounding,
							element->progressBorderSize,
							element->progressUseColors,
							element->progressBg,
							element->progressFill,
							element->progressBorder,
							element->progressCapMax,
							element->progressCapFill,
							element->progressOverlayText.c_str()
						);
					}
					else
					{
						m_ui->DrawProgressBarStyled(
							element->xPos, element->yPos,
							element->xSize, element->ySize,
							element->maxValue,
							element->progressCurrentValue,
							element->progressColorId,
							element->progressRounding,
							element->progressBorderSize,
							element->progressUseColors,
							element->progressBg,
							element->progressFill,
							element->progressBorder,
							element->progressOverlayText.c_str()
						);
					}
				}
				else
				{
					m_ui->DrawProgressBar(
						element->xPos, element->yPos,
						element->xSize, element->ySize,
						element->maxValue,
						element->progressCurrentValue,
						element->progressColorId
					);
				}
			}
			break;
		}

		case UIType::Dropdown:
		{
			if (element->widgetId.empty()) break;

			int& idx = m_dropdownIndex[element->widgetId];

			// clamp safety
			if (!element->dropdownOptions.empty())
			{
				if (idx < 0 || idx >= (int)element->dropdownOptions.size())
					idx = 0;
			}
			else idx = 0;

			std::string imguiLabel = element->ownedText + "##" + element->widgetId;

			int pushedColors = 0;
			int pushedVars = 0;
			if (element->dropdownCustomStyle)
			{
				if (element->dropdownRounding >= 0.0f)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, element->dropdownRounding);
					pushedVars++;
				}
				if (element->dropdownPopupRounding >= 0.0f)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, element->dropdownPopupRounding);
					pushedVars++;
				}
				if (element->dropdownBorderSize >= 0.0f)
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, element->dropdownBorderSize);
					ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, element->dropdownBorderSize);
					pushedVars += 2;
				}

				if (element->ddHasFrame) { ImGui::PushStyleColor(ImGuiCol_FrameBg, element->ddFrame); pushedColors++; }
				if (element->ddHasFrameHovered) { ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, element->ddFrameHovered); pushedColors++; }
				if (element->ddHasFrameActive) { ImGui::PushStyleColor(ImGuiCol_FrameBgActive, element->ddFrameActive); pushedColors++; }

				if (element->ddHasPopupBg) { ImGui::PushStyleColor(ImGuiCol_PopupBg, element->ddPopupBg); pushedColors++; }
				if (element->ddHasBorder) { ImGui::PushStyleColor(ImGuiCol_Border, element->ddBorder); pushedColors++; }

				// Selectable colors inside combo popup
				if (element->ddHasItem) { ImGui::PushStyleColor(ImGuiCol_Header, element->ddItem); pushedColors++; }
				if (element->ddHasItemHovered) { ImGui::PushStyleColor(ImGuiCol_HeaderHovered, element->ddItemHovered); pushedColors++; }
				if (element->ddHasItemActive) { ImGui::PushStyleColor(ImGuiCol_HeaderActive, element->ddItemActive); pushedColors++; }

				if (element->ddHasText) { ImGui::PushStyleColor(ImGuiCol_Text, element->ddText); pushedColors++; }
			}

			bool changed = m_ui->DrawDropdown(element->xPos, element->yPos,
				element->xSize, element->ySize,
				imguiLabel.c_str(),
				element->dropdownOptions, &idx);

			if (pushedColors) ImGui::PopStyleColor(pushedColors);
			if (pushedVars) ImGui::PopStyleVar(pushedVars);

			if (changed)
				m_dropdownChanged[element->widgetId] = true;

			break;
		}

		case UIType::Panel:
			m_ui->DrawPanel(element->xPos, element->yPos, element->xSize, element->ySize,
				element->panelAlpha, element->panelRounding,
				element->panelR, element->panelG, element->panelB);
			break;

		case UIType::BeginChild:
		{
			ChildStylePop pops{};

			ImVec4 bgCol(0, 0, 0, 0);
			if (element->childHasBg)
			{
				bgCol = ImVec4(
					element->childBgR / 255.0f,
					element->childBgG / 255.0f,
					element->childBgB / 255.0f,
					element->childBgAlpha
				);

				ImGui::PushStyleColor(ImGuiCol_ChildBg, bgCol);
				pops.colors++;

				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, element->childBgRounding);
				pops.vars++;
			}

			// Border thickness + color
			if (element->childBorder)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, element->childBorderSize);
				pops.vars++;

				ImVec4 borderCol = element->childBorderColor;

				// Auto: slightly darker than background (only makes sense if bg exists)
				if (element->childAutoBorderColor && element->childHasBg)
				{
					float f = element->childBorderDarken;
					borderCol = ImVec4(bgCol.x * f, bgCol.y * f, bgCol.z * f, bgCol.w);
				}

				ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
				pops.colors++;
			}

			m_childStyleStack.push_back(pops);

			ImGui::SetCursorPos(ImVec2((float)element->xPos, (float)element->yPos));
			ImGui::BeginChild(
				element->childId.c_str(),
				ImVec2(element->xSize, element->ySize),
				element->childBorder,
				element->childFlags
			);
			break;
		}


		case UIType::EndChild:
		{
			ImGui::EndChild();

			if (!m_childStyleStack.empty())
			{
				ChildStylePop pops = m_childStyleStack.back();
				m_childStyleStack.pop_back();
				if (pops.vars)   ImGui::PopStyleVar(pops.vars);
				if (pops.colors) ImGui::PopStyleColor(pops.colors);
			}
			break;
		}


		default:
			break;
		}

		if (element->fontScale != 1.0f)
			ImGui::SetWindowFontScale(1.0f);
		// --- FONT POP ---
		if (font) ImGui::PopFont();
		if (element->sameline) ImGui::SameLine();
	}

	void UIManager::RenderUI()
	{
		// clear per-frame results
		m_inputCommittedThisFrame.clear();
		m_buttonPressed.clear();
		m_checkboxChanged.clear();
		m_sliderChanged.clear();
		m_radioChanged.clear();
		m_dropdownChanged.clear();
		m_colorChanged.clear();

		for (auto* element : elements)
		{
			RenderElement(element);
			delete element;
		}
		elements.clear();
	}
}