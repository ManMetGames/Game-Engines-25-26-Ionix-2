#include "UIManager.h"
#include <iostream>
#include <imgui.h>


void IonixEngine::UIManager::BeginPanel(const std::string& panelName)
{

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
}

void IonixEngine::UIManager::EndPanel()
{
	ImGui::EndChild();
	return;
}