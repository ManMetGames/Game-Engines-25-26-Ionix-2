#include "UIManager.h"
#include <iostream>
#include <imgui.h>

namespace IonixEngine
{


void UIManager::BeginPanel(const std::string& panelName)
{
	UIElement* element = new UIElement{};
	element->type = UIType::Panel;
	elements.push_back(element);
}

void UIManager::AddChildToPanel(UIElement* element)
{
	elements.push_back(element);
}

void UIManager::AddLabel(int x, int y, float xSize, float ySize, const char* text, const std::string& fontName)
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
	elements.push_back(element);
	AddChildToPanel(element);
}

void UIManager::AddButton(int x, int y, float xSize, float ySize, const char* text)
{
	UIElement* element = new UIElement;
	element->type = UIType::Button;
	element->xPos = x;
	element->yPos = y;
	element->xSize = xSize;
	element->ySize = ySize;
	element->ownedText = (text ? text : "");
	element->text = const_cast<char*>(element->ownedText.c_str());
	AddChildToPanel(element);
}

void UIManager::AddCheckbox(int x, int y, float xSize, float ySize, const char* text, bool* checked, const std::string& fontName)
{
	UIElement* element = new UIElement;
	element->type = UIType::Checkbox;
	element->xPos = x;
	element->yPos = y;
	element->xSize = xSize;
	element->ySize = ySize;
	element->ownedText = (text ? text : "");
	element->text = const_cast<char*>(element->ownedText.c_str());
	element->checked = checked;
	element->fontName = fontName;
	AddChildToPanel(element);
}

void UIManager::AddSliderFloat(int x, int y, float xSize, float ySize, const char* text, float* value, float min, float max, const std::string& fontName)
{
	UIElement* element = new UIElement;
	element->type = UIType::SliderFloat;
	element->xPos = x;
	element->yPos = y;
	element->xSize = xSize;
	element->ySize = ySize;
	element->ownedText = (text ? text : "");
	element->text = const_cast<char*>(element->ownedText.c_str());
	element->sliderValue = value;
	element->sliderMin = min;
	element->slidermax = max;
	element->fontName = fontName;
	AddChildToPanel(element);
}

void UIManager::AddInputText(int xPos, int yPos, float width, const char* label, const char* id, size_t maxLen)
{
	UIElement* element = new UIElement;
	element->type = UIType::InputText;
	element->xPos = xPos;
	element->yPos = yPos;
	element->width = width;
	element->ownedText = (label ? label : "");
	element->text = const_cast<char*>(element->ownedText.c_str());
	element->inputId = id;

	auto& buf = m_inputBuffers[element->inputId];
	if (buf.empty())
		buf.assign(maxLen, '\0');   // persistent storage
	element->inputBuffer = buf.data();
	element->inputBufferSize = 16;
	element->inputBufferSize = buf.size();
	const std::string fontName;
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

void UIManager::AddRadioButton(int x, int y, float xSize, float ySize, const char* text, int* radioValuePointer, int value, bool sameline, const std::string& fontName)
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
	AddChildToPanel(element);
}

void UIManager::AddColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color, const std::string& fontName)
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
	AddChildToPanel(element);
}

void UIManager::AddDropdown(int x, int y, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex, const std::string& fontName)
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

void UIManager::EndPanel()
{
	ImGui::EndChild();
	return;
}

void UIManager::ClearElements()
{
	elements.clear();
}

void UIManager::RenderElement(UIElement* element)
{
	// --- FONT PUSH ---
	ImFont* font = nullptr;
	if (!element->fontName.empty())
	{
		font = fontLoader.GetFont(element->fontName);
	}
	if (font)
	{
		ImGui::PushFont(font);
	}
	switch (element->type)
	{
	case UIType::Label:

		m_ui->DrawLabel(element->text, element->xSize, element->ySize, element->xPos, element->yPos, element->fontName);
		break;
	case UIType::Button:
		m_ui->DrawButton(element->text, element->xSize, element->ySize, element->xPos, element->yPos);
		break;
	case UIType::Checkbox:
		if (element->checked)
			m_ui->DrawCheckbox(0, element->text, element->xPos, element->yPos, element->xSize, element->ySize);
		break;
	case UIType::SliderFloat:
		if (element->sliderValue)
			*element->sliderValue = m_ui->DrawSlider(element->text, *element->sliderValue, element->xSize, element->ySize, element->xPos, element->yPos, element->sliderMin, element->slidermax);
		break;
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
		// --- FONT POP ---
		if (font)
		{
			ImGui::PopFont();
		}

		if (element->sameline)
			ImGui::SameLine();
	}
}

void UIManager::RenderUI()
{
	m_inputCommittedThisFrame.clear();
	for (auto& element : elements)
	{
		RenderElement(element);
	}
	//for (auto* e : elements) delete e;
	elements.clear();
}

}

