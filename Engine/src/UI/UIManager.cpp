#include "UIManager.h"
#include <iostream>
#include <imgui.h>



void IonixEngine::UIManager::BeginGroup(const std::string& groupName)
{
	currentGroupName = groupName;
}

void IonixEngine::UIManager::EndGroup()
{
	currentGroupName.clear();
}

void IonixEngine::UIManager::BeginPanel(const std::string& panelName)
{
	UIElement panel;
	panel.type = UIType::Panel;
	panel.groupName = panelName;
	panel.isChildGroup = true;

	if (!groupStack.empty())
	{
		groupStack.back()->children.push_back(panel);
	}
	else
	{
		elements.push_back(panel);
	}

	if (!groupStack.empty())
	{
		groupStack.push_back(&groupStack.back()->children.back());
	}
	else
	{
		groupStack.push_back(&elements.back());
	}
}

void IonixEngine::UIManager::EndPanel()
{
	if (!groupStack.empty())
	{
		groupStack.pop_back();
	}
}

void IonixEngine::UIManager::AddChildToPanel(UIElement element)
{
	if (!groupStack.empty())
	{
		groupStack.back()->children.push_back(element);
	}
	else
	{
		elements.push_back(element);
	}
}


void IonixEngine::UIManager::AddLabel(int x, int y, float xSize, float ySize, const char* text)
{
	UIElement element;
	element.type = UIType::Label;
	element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddButton(int x, int y, float xSize, float ySize, const char* text, std::function<void()> onClick)
{
	UIElement element;
	element.type = UIType::Button;
	element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.onClick = onClick;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddCheckbox(int x, int y, float xSize, float ySize, const char* text, bool* checked)
{
	UIElement element;
	element.type = UIType::Checkbox;
	element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.checked = checked;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddSliderFloat(int x, int y, float xSize, float ySize, const char* text, float* value, float min, float max)
{
	UIElement element;
	element.type = UIType::SliderFloat;
	element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.sliderValue = value;
	element.sliderMin = min;
	element.slidermax = max;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddInputText(int x, int y, float xSize, float ySize, const char* text, char* buffer, size_t bufferSize)
{
	UIElement element;
	element.type = UIType::InputText;
	element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.inputBuffer = buffer;
	element.inputBufferSize = bufferSize;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddRadioButton(int x, int y, float xSize, float ySize, const char* text, int* radioValuePointer, int value, bool sameline)
{
	UIElement element;
	element.type = UIType::RadioButton;
	element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.radioValuePtr = radioValuePointer;
	element.RadioButtonValue = value;
	element.sameline = sameline;
	AddChildToPanel(element);
}
void IonixEngine::UIManager::AddColorPicker3(int x, int y, float xSize, float ySize, const char* label, float* color)
{
	UIElement element;
	element.type = UIType::ColorPicker3;
	element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(label);
	element.color3 = color;
	AddChildToPanel(element);
}


void IonixEngine::UIManager::AddColorPicker4(int x, int y, float xSize, float ySize, const char* label, float* color)
{
	UIElement element;
	element.type = UIType::ColorPicker4;
	element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(label);
	element.color4 = color;
	AddChildToPanel(element);
}

void IonixEngine::UIManager::AddDropdown(int x, int y, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex)
{
	UIElement element;
	element.type = UIType::Dropdown;
	element.groupName = currentGroupName;
	element.xPos = x;
	element.yPos = y;
	element.xSize = xSize;
	element.ySize = ySize;
	element.text = const_cast<char*>(text);
	element.dropdownOptions = options;	
	element.dropdownCurrentIndex = currentIndex;

	AddChildToPanel(element);
}

void IonixEngine::UIManager::RenderElement(UIElement& element)
{
	if (element.type == UIType::Panel)
	{
		ImGui::SetCursorPos(ImVec2(element.xPos, element.yPos));
		ImGui::BeginChild(element.groupName.c_str(), ImVec2(element.xSize, element.ySize), true);
		for (auto& child : element.children)
		{
			RenderElement(child);
		}
		ImGui::EndChild();
		return;
	}

	ImGui::SetCursorPos(ImVec2((float)element.xPos, (float)element.yPos));
	switch (element.type)
	{
	case UIType::Label:
		ImGui::Text("%s", element.text);
		break;
	case UIType::Button:
		if (ImGui::Button(element.text))
		{
			if (element.onClick)
				element.onClick();
		}
		break;
	case UIType::Checkbox:
		if (element.checked)
			ImGui::Checkbox(element.text, element.checked);
		break;
	case UIType::SliderFloat:
		if (element.sliderValue)
			ImGui::SliderFloat(element.text, element.sliderValue, element.sliderMin, element.slidermax);
		break;
	case UIType::InputText:
		if (element.inputBuffer)
			ImGui::InputText(element.text, element.inputBuffer, element.inputBufferSize);
		break;
	case UIType::RadioButton:
		if (element.radioValuePtr)
		{
			if (ImGui::RadioButton(element.text, *element.radioValuePtr == element.RadioButtonValue))
			{
				*element.radioValuePtr = element.RadioButtonValue;
			}
		}
		break;
	case UIType::Dropdown:
		ImGui::SetCursorPos(ImVec2(element.xPos, element.yPos));
		int currentIndex = *(element.dropdownCurrentIndex);
		const char* currentItem = element.dropdownOptions[currentIndex].c_str();
		if (ImGui::BeginCombo(element.text, currentItem))
		{
			for (size_t n = 0; n < element.dropdownOptions.size(); n++)
			{
				bool isSelected = (currentIndex == n);
				if (ImGui::Selectable(element.dropdownOptions[n].c_str(), isSelected))
				{
					*(element.dropdownCurrentIndex) = n;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
			
		break;
	

		if (element.sameline)
			ImGui::SameLine();
	}
}
void IonixEngine::UIManager::RenderUI()
{
	;
	for (auto& element : groupStack.front()->children)
	{
		RenderElement(element);
	}

	

}

