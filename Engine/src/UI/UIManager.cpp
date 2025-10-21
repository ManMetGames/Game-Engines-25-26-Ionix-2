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


void IonixEngine::UIManager::AddLabel(int x, int y, float xSize, float ySize, const char* text)
{
	elements.push_back({ UIType::Label,currentGroupName , x, y, xSize, ySize, const_cast<char*>(text), nullptr });
}

void IonixEngine::UIManager::AddButton(int x, int y, float xSize, float ySize, const char* text, std::function<void()> onClick)
{
	elements.push_back({ UIType::Button,currentGroupName, x, y, xSize, ySize, const_cast<char*>(text), onClick });
}

void IonixEngine::UIManager::AddCheckbox(int x, int y, float xSize, float ySize, const char* text, bool* checked)
{
	elements.push_back({ UIType::Checkbox,currentGroupName, x, y, xSize, ySize, const_cast<char*>(text), nullptr, checked });
}

void IonixEngine::UIManager::AddSliderFloat(int x, int y, float xSize, float ySize, const char* text, float* value, float min, float max)
{
	elements.push_back({ UIType::SliderFloat,currentGroupName, x, y, xSize, ySize, const_cast<char*>(text), nullptr, nullptr, value, min, max });
}

void IonixEngine::UIManager::AddInputText(int x, int y, float xSize, float ySize, const char* text, char* buffer, size_t bufferSize)
{
	elements.push_back({ UIType::InputText,currentGroupName, x, y, xSize, ySize, const_cast<char*>(text), nullptr, nullptr, nullptr, 0.0f, 0.0f, buffer, bufferSize });
}

void IonixEngine::UIManager::AddRadioButton(int x, int y, const char* text, int* radioValuePointer, int value, bool sameline)
{
	elements.push_back({ UIType::RadioButton,currentGroupName, x, y, 0.0, 0.0, const_cast<char*>(text), nullptr, nullptr, nullptr, 0.0f, 0.0f, nullptr, 0, radioValuePointer, value, sameline});
}

void IonixEngine::UIManager::RenderElement(UIElement& element)
{
	if (element.type == UIType::Panel)
	{
		ImGui::BeginChild(element.groupName.c_str(), ImVec2(0, 0), true);
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
	default:
		break;
	}
}
void IonixEngine::UIManager::RenderUI()
{
	std::string currentGroup = "";
	for (auto& element : elements)
	{
		if (element.groupName != currentGroup)
		{
			if (!currentGroup.empty())
			{
				ImGui::End();
			}
			ImGui::Begin(element.groupName.c_str());
			currentGroup = element.groupName;
			
		}
		RenderElement(element);
	}

	if (!currentGroup.empty())
	{
		ImGui::End();
	}

}

