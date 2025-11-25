#pragma once
#include <imgui.h>
#include <string>
#include <functional>
#include <vector>
#include "FontLoader.h"

#include "UI/UI.h"
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
		ProgressBar,
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

		// Font name for this element
		std::string fontName;

		float maxValue = 0.0f; // only for ProgressBar
		float* currentValue = nullptr; // only for ProgressBar
		float incrementAmount = 0.0f; // only for ProgressBar
		std::vector<UIElement> children;
		bool isChildGroup = false;

		std::vector<std::string> dropdownOptions;
		int* dropdownCurrentIndex = nullptr;

	};

	class UIManager
	{
	private:
		//std::string currentGroupName; 

		//std::vector<UIElement*> groupStack; 

		void RenderElement(UIElement& element);

		UI* m_ui = nullptr;
	public:
		Fontloader fontLoader;
		std::vector<UIElement> GetElements()
		{
			return elements;
		}

		std::vector<UIElement> elements;
		void setUIRenderer(UI* ui)
		{
			m_ui = ui;
		}
		//void UIManager::BeginGroup(const std::string& groupName);

		//void UIManager::EndGroup();

		void BeginPanel(const std::string& panelName);

		void EndPanel();

		void AddChildToPanel(UIElement element);

		// Add for new UITypes below

		void AddLabel(int x, int y, float xSize, float ySize, const char* text, const std::string& fontName = "");

		void AddButton(int x, int y, float xSize, float ySize, const char* text, std::function<void()> onClick, const std::string& fontName = "");

		void AddCheckbox(int x, int y, float xSize, float ySize, const char* text, bool* checked, const std::string& fontName = "");

		void AddSliderFloat(int x, int y, float xSize, float ySize, const char* text, float* value, float min, float max, const std::string& fontName = "");

		void AddInputText(int x, int y, float xSize, float ySize, const char* text, char* buffer, size_t bufferSize, const std::string& fontName = "");

		void AddRadioButton(int x, int y, float xSize, float ySize, const char* text, int* radioValuePointer, int value, bool sameline = false, const std::string& fontName = "");

		void AddDropdown(int x, int y, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex, const std::string& fontName = "");

		void AddColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color, const std::string& fontName = "");

		void ClearElements();

		void AddProgressBar(int x, int y, float xSize, float ySize, float maxvalue, float* currentvalue, float incrementamount, const std::string& fontName = "");

		void RenderUI();

	};
}
