#include "UIManager.h"
#include <iostream>
#include <imgui.h>

namespace IonixEngine
{


void UIManager::BeginPanel(const std::string& panelName)
{

}

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
	element->text = const_cast<char*>(element->ownedText.c_str());
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
	element->text = const_cast<char*>(element->ownedText.c_str());

	element->fontName = fontName;
	AddChildToPanel(element);
}

void UIManager::AddButton(int x, int y, float xSize, float ySize, const char* text, const char* id, const std::string& fontName, float fontScale)
{
	UIElement* element = new UIElement{};
	element->type = UIType::Button;
	element->xPos = x;
	element->yPos = y;
	element->xSize = xSize;
	element->ySize = ySize;

	element->fontName = fontName;
	element->fontScale = fontScale;

	element->ownedText = (text ? text : "");
	element->text = const_cast<char*>(element->ownedText.c_str());

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
	element->text = const_cast<char*>(element->ownedText.c_str());

	element->fontName = fontName;
	element->fontScale = fontScale;

	element->widgetId = (id && id[0]) ? id : element->ownedText;
	element->defaultValue = defaultValue;

	// only set default once
	if (m_checkboxValues.find(element->widgetId) == m_checkboxValues.end())
		m_checkboxValues[element->widgetId] = defaultValue;

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
	float min, float max, float defaultValue, const std::string& fontName, float fontScale)
{
	UIElement* element = new UIElement{};
	element->type = UIType::SliderFloat;
	element->xPos = x;
	element->yPos = y;
	element->xSize = width;          // use xSize as width
	element->ySize = 0.0f;

	element->ownedText = (label ? label : "");
	element->text = const_cast<char*>(element->ownedText.c_str());

	element->fontName = fontName;
	element->fontScale = fontScale;

	element->widgetId = (id && id[0]) ? id : element->ownedText;
	element->sliderMin = min;
	element->slidermax = max;

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
	element->text = const_cast<char*>(element->ownedText.c_str());
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

void UIManager::ClearInput(const std::string& id)
{
	// clear committed text
	m_committedText[id].clear();

	// clear live buffer
	auto it = m_inputBuffers.find(id);
	if (it != m_inputBuffers.end() && !it->second.empty())
		it->second[0] = '\0';
}

void UIManager::AddRadioButton(int x, int y, float xSize, float ySize, const char* text, int* radioValuePointer, int value, bool sameline, const std::string& fontName, float fontScale)
{
	UIElement* element = new UIElement;
	element->type = UIType::RadioButton;
	element->xPos = x;
	element->yPos = y;
	element->xSize = xSize;
	element->ySize = ySize;
	element->ownedText = (text ? text : "");
	element->text = const_cast<char*>(element->ownedText.c_str());
	element->radioValuePtr = radioValuePointer;
	element->RadioButtonValue = value;
	element->sameline = sameline;
	element->fontName = fontName;
	element->fontScale = fontScale;
	AddChildToPanel(element);
}

void UIManager::AddColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color, const std::string& fontName, float fontScale)
{
	UIElement* element = new UIElement;
	element->type = UIType::ColorPicker;
	element->xPos = x;
	element->yPos = y;
	element->xSize = xSize;
	element->ySize = ySize;
	element->ownedText = (label ? label : "");
	element->text = const_cast<char*>(element->ownedText.c_str());
	element->color = color;
	element->fontName = fontName;
	element->fontScale = fontScale;
	AddChildToPanel(element);
}

void UIManager::AddDropdown(int x, int y, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex, const std::string& fontName, float fontScale)
{
	UIElement* element = new UIElement;
	element->type = UIType::Dropdown;
	element->xPos = x;
	element->yPos = y;
	element->xSize = xSize;
	element->ySize = ySize;
	element->ownedText = (text ? text : "");
	element->text = const_cast<char*>(element->ownedText.c_str());
	element->dropdownOptions = options;
	element->dropdownCurrentIndex = currentIndex;
	element->fontName = fontName;
	element->fontScale = fontScale;
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


void UIManager::EndPanel()
{
	ImGui::EndChild();
	return;
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

		m_ui->DrawLabel(element->text, element->xSize, element->ySize, (int)x, element->yPos, element->fontName);
		break;
	}
	case UIType::Button:
	{
		// Make ImGui id unique: "Visible##id"
		std::string imguiLabel = element->ownedText + "##" + element->widgetId;

		bool pressed = m_ui->DrawButton(
			const_cast<char*>(imguiLabel.c_str()),
			(int)element->xSize, (int)element->ySize,
			element->xPos, element->yPos
		);

		if (pressed)
			m_buttonPressed[element->widgetId] = true;

		break;
	}

	case UIType::Checkbox:
	{
		if (element->widgetId.empty())
			break;

		bool& v = m_checkboxValues[element->widgetId];

		std::string imguiLabel = element->ownedText + "##" + element->widgetId;

		bool changed = m_ui->DrawCheckbox(imguiLabel.c_str(), &v, element->xPos, element->yPos);

		if (changed)
			m_checkboxChanged[element->widgetId] = true;

		break;
	}


	case UIType::SliderFloat:
	{
		if (element->widgetId.empty())
			break;

		float& v = m_sliderValues[element->widgetId];

		// "Visible label##id" trick (same as your button/checkbox)
		std::string imguiLabel = element->ownedText + "##" + element->widgetId;

		bool changed = m_ui->DrawSlider(
			imguiLabel.c_str(),
			&v,
			element->xSize,          // width
			element->xPos, element->yPos,
			element->sliderMin, element->slidermax
		);

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
		if (element->radioValuePtr)
		{
			m_ui->DrawRadioButton(element->xPos, element->yPos, element->text, *element->radioValuePtr, element->RadioButtonValue, element->sameline);
		}
		break;
	case UIType::ColorPicker:
		if (element->color)
			m_ui->DrawColorPicker(element->xPos, element->yPos, element->xSize, element->ySize, element->text, element->color);
		break;
	case UIType::ProgressBar:
		if (element->currentValue)
		{
			m_ui->DrawProgressBar(element->xPos, element->yPos, element->xSize, element->ySize, element->maxValue, *element->currentValue, element->incrementAmount);
		}
		break;
	case UIType::Dropdown:
		if (element->dropdownCurrentIndex)
		{
			m_ui->DrawDropdown(element->xPos, element->yPos, element->ySize, element->xSize, element->text, element->dropdownOptions, element->dropdownCurrentIndex);
		}
		break;

	case UIType::Panel:
		m_ui->DrawPanel(element->xPos, element->yPos, element->xSize, element->ySize,
			element->panelAlpha, element->panelRounding,
			element->panelR, element->panelG, element->panelB);
		break;

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

	for (auto* element : elements)
	{
		RenderElement(element);
		delete element;
	}
	elements.clear();
}
}