#include "UIManager.h"
#include <iostream>
#include <imgui.h>
#include <string>


/*
void IonixEngine::UIManager::BeginGroup(const std::string& groupName)
{
	currentGroupName = groupName;
}
*/
/*
void IonixEngine::UIManager::EndGroup()
{
	currentGroupName = std::string("");
}
*/
void IonixEngine::UIManager::BeginPanel(const std::string& panelName)
{
	UIElement element;
	element.type = UIType::Panel;
	elements.push_back(element);
	//panel.groupName = panelName;
	//panel.isChildGroup = true;

	//if (!groupStack.empty())
	//{
	//	groupStack.back()->children.push_back(panel);
	//}
	//else
	//{
		//elements.push_back(element);
	//}

	//if (!groupStack.empty())
	//{
	//	groupStack.push_back(&groupStack.back()->children.back());
	//}
	//else
	//{
	//	groupStack.push_back(&elements.back());
	//}
}

void IonixEngine::UIManager::AddChildToPanel(UIElement element)
{
	elements.push_back(element);	
}


void IonixEngine::UIManager::AddLabel(int x, int y, float xSize, float ySize, const char* text, const std::string& fontName)
{
	UIElement element;
	element.type = UIType::Label;
	//element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.fontName = fontName;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddButton(int x, int y, float xSize, float ySize, const char* text, std::function<void()> onClick, const std::string& fontName)
{
	UIElement element;
	element.type = UIType::Button;
	//element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.onClick = onClick;
	element.fontName = fontName;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddCheckbox(int x, int y, float xSize, float ySize, const char* text, bool* checked, const std::string& fontName)
{

	UIElement element;
	element.type = UIType::Checkbox;
	//element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.checked = checked;
	element.fontName = fontName;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddSliderFloat(int x, int y, float xSize, float ySize, const char* text, float* value, float min, float max, const std::string& fontName)
{
	UIElement element;
	element.type = UIType::SliderFloat;
	//element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.sliderValue = value;
	element.sliderMin = min;
	element.slidermax = max;
	element.fontName = fontName;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddInputText(int x, int y, float xSize, float ySize, const char* text, char* buffer, size_t bufferSize, const std::string& fontName)
{
	UIElement element;
	element.type = UIType::InputText;
	//element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.inputBuffer = buffer;
	element.inputBufferSize = bufferSize;
	element.fontName = fontName;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddRadioButton(int x, int y, float xSize, float ySize, const char* text, int* radioValuePointer, int value, bool sameline, const std::string& fontName)
{
	UIElement element;
	element.type = UIType::RadioButton;
	//element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.radioValuePtr = radioValuePointer;
	element.RadioButtonValue = value;
	element.sameline = sameline;
	element.fontName = fontName;
	AddChildToPanel(element);
}



void IonixEngine::UIManager::AddColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color, const std::string& fontName)
{
	UIElement element;
	element.type = UIType::ColorPicker;
	//element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(label);
	element.color = color;
	element.fontName = fontName;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::ClearElements()
{
	elements.clear();
}

void IonixEngine::UIManager::AddDropdown(int x, int y, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex, const std::string& fontName)
{
	UIElement element;
	element.type = UIType::Dropdown;
	//element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.dropdownOptions = options;	
	element.dropdownCurrentIndex = currentIndex;
	element.fontName = fontName;

	AddChildToPanel(element);
}
void IonixEngine::UIManager::AddProgressBar(int x, int y, float xSize, float ySize, float maxvalue, float* currentvalue, float incrementamount, const std::string& fontName)
{
	UIElement element;
	element.type = UIType::ProgressBar;
	//element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.maxValue = maxvalue;
	element.currentValue = currentvalue;
	element.incrementAmount = incrementamount;
	element.fontName = fontName;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::EndPanel()
{
	ImGui::EndChild();
	return;
}

void IonixEngine::UIManager::RenderElement(UIElement& element)
{
	//ImGui::SetCursorPos(ImVec2(element.xPos, element.yPos));
	
	//if (element.type == UIType::Panel)
	//{
		//ImGui::SetCursorPos(ImVec2(element.xPos, element.yPos));
		/*
		for (auto& child : element.children)
		{
			RenderElement(child);
		}
		*/
		//ImGui::EndChild();
		//return;
	//}
	
	ImGui::Render;
	ImGui::SetCursorPos(ImVec2((float)element.xPos, (float)element.yPos));
	// --- FONT PUSH ---
	ImFont* font = nullptr;
	if (!element.fontName.empty())
	{
		font = fontLoader.GetFont(element.fontName);
	}
	if (font)
	{
		ImGui::PushFont(font);
	}
	switch (element.type)
	{
	case UIType::Label:
			m_ui->DrawLabel(element.text, element.xSize, element.ySize, element.xPos, element.yPos, element.fontName);
		break;
	case UIType::Button:
		if (m_ui->DrawButton(element.text, element.xSize, element.ySize, element.xPos, element.yPos))
		{
			if (element.onClick)
				element.onClick();
		}
		break;
	case UIType::Checkbox:
		if (element.checked)
			m_ui->DrawCheckbox(0, element.text, element.xPos, element.yPos, element.xSize, element.ySize);
		break;
	case UIType::SliderFloat:
		if (element.sliderValue)
			*element.sliderValue = m_ui->DrawSlider(element.text, *element.sliderValue, element.xSize, element.ySize, element.xPos, element.yPos, element.sliderMin, element.slidermax); 
		break;
	case UIType::InputText:
		if (element.inputBuffer)
		{
			ImGui::SetCursorPos(ImVec2(element.xPos, element.yPos));
			ImGui::InputText(element.text, element.inputBuffer, element.inputBufferSize);		
		}
			break;
	case UIType::RadioButton:
		if (element.radioValuePtr)
		{
			m_ui->DrawRadioButton(element.xPos, element.yPos, element.text, *element.radioValuePtr, element.RadioButtonValue, element.sameline);
		}
		break;
	case UIType::ColorPicker:
		if (element.color)
			m_ui->DrawColorPicker(element.xPos, element.yPos, element.xSize, element.ySize, element.text, element.color);
		break;
	case UIType::ProgressBar:
		if (element.currentValue)
		{
			m_ui->DrawProgressBar(element.xPos, element.yPos, element.xSize, element.ySize, element.maxValue, *element.currentValue, element.incrementAmount);
		}
		break;
	case UIType::Dropdown:
		if (element.dropdownCurrentIndex)
		{
			m_ui->DrawDropdown(element.xPos, element.yPos, element.ySize, element.xSize, element.text, element.dropdownOptions, element.dropdownCurrentIndex);
		}
		break;
		// --- FONT POP ---
		if (font)
		{
			ImGui::PopFont();
		}

		if (element.sameline)
			ImGui::SameLine();
	}
}

void IonixEngine::UIManager::RenderUI()
{
	;
	for (auto& element : elements)
	{
		RenderElement(element);

	}
}

