#pragma once
#include <imgui.h>
#include <string>
#include <functional>
#include <vector>
namespace IonixEngine
{
	enum UIType
	{
		Label,
		Button,
		Checkbox,
		SliderFloat,
		InputText,
		Panel,
		RadioButton,
		Dropdown,
		ColorPicker,
	};
	struct UIElement
	{
		UIType type;
		std::string groupName;
		int xPos;
		int yPos;
		float xSize;
		float ySize;
		char* text = nullptr;
		std::function<void()> onClick = nullptr; // only for buttons
		bool* checked = nullptr; // only for checkboxes
		float* sliderValue = nullptr; // only for sliders
		float sliderMin = 0.0f;// only for sliders
		float slidermax = 1.0f;// only for sliders
		char* inputBuffer = nullptr; // only for input text
		size_t inputBufferSize; // only for input text
		int* radioValuePtr = nullptr;
		int RadioButtonValue = 0;
		bool sameline = false;
		float* color = nullptr; // only for ColorPicker4
		
		std::vector<UIElement> children;
		bool isChildGroup = false;

		std::vector<std::string> dropdownOptions;
		int* dropdownCurrentIndex = nullptr;
		
	};

	class UIManager
	{
	private: 
		std::string currentGroupName; 

		std::vector<UIElement*> groupStack; 

		void RenderElement(UIElement& element);
	public:

		std::vector<UIElement> GetElements()
		{
			return elements;
		}

		std::vector<UIElement> elements;
		void UIManager::BeginGroup(const std::string& groupName);
		
		void UIManager::EndGroup();

		void UIManager::BeginPanel(const std::string& panelName);

		void UIManager::EndPanel();

		void UIManager::AddChildToPanel(UIElement element);
		
		// Add for new UITypes below
		void AddLabel(int x, int y, float xSize, float ySize, const char* text);
		
		void AddButton(int x, int y, float xSize, float ySize, const char* text, std::function<void()> onClick);
		
		void AddCheckbox(int x, int y, float xSize, float ySize, const char* text, bool* checked);
		
		void AddSliderFloat(int x, int y, float xSize, float ySize, const char* text, float* value, float min, float max);
		
		void AddInputText(int x, int y, float xSize, float ySize, const char* text, char* buffer, size_t bufferSize);
		
		void AddRadioButton(int x, int y, float xSize, float ySize, const char* text, int* radioValuePointer, int value, bool sameline);
		
		void AddDropdown(int x, int y, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex);
		
		
		void AddColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color);

		void RenderUI();
		
	};
}

