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
		int* radioButtonValue = nullptr;
		int radioButtonCurrValue = 0;
		bool sameline;

		std::vector<UIElement> children;
		bool isChildGroup = false;
		
	};

	class UIManager
	{
	private: 
		std::string currentGroupName; 
		std::vector<UIElement> elements;
		std::vector<UIElement*> groupStack; 

		void RenderElement(UIElement& element);
	public:
		void UIManager::BeginGroup(const std::string& groupName);
		
		void UIManager::EndGroup();

		void UIManager::BeginPanel(const std::string& panelName);

		void UIManager::EndPanel();
		
		// Add for new UITypes below
		void AddLabel(int x, int y, float xSize, float ySize, const char* text);
		
		void AddButton(int x, int y, float xSize, float ySize, const char* text, std::function<void()> onClick);
		
		void AddCheckbox(int x, int y, float xSize, float ySize, const char* text, bool* checked);
		
		void AddSliderFloat(int x, int y, float xSize, float ySize, const char* text, float* value, float min, float max);
		
		void AddInputText(int x, int y, float xSize, float ySize, const char* text, char* buffer, size_t bufferSize);
		
		void AddRadioButton(int x, int y, float xSize, float ySize, const char* text, int* e, int value, bool sameline);

		void RenderUI();
		
	};
}

