#pragma once
#include <imgui.h>
#include <string>
#include <functional>
#include <vector>
#include "Fontloader.h"
#include "UI/UI.h"
#include <unordered_map>

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
		//std::string groupName;
		int xPos;
		int yPos;
		float xSize;
		float ySize;
		std::string ownedText;
		char* text = nullptr;
		//std::function<void()> onClick = nullptr; // only for buttons
		bool* checked = nullptr; // only for checkboxes
		float* sliderValue = nullptr; // only for sliders
		float sliderMin = 0.0f;// only for sliders
		float slidermax = 1.0f;// only for sliders
		int* radioValuePtr = nullptr;
		int RadioButtonValue = 0;
		bool sameline = false;
		float* color = nullptr; // only for ColorPicker4


		char* inputBuffer = nullptr; // only for InputText
		size_t inputBufferSize = 0; //only for InputText
		float width = 100.0f; // only for InputText
		std::string inputId;   // only for InputText

		// Font name for this element
		std::string fontName;
		float maxValue = 0.0f; // only for ProgressBar
		float* currentValue = nullptr; // only for ProgressBar
		float incrementAmount = 0.0f; // only for ProgressBar
		std::vector<UIElement> children;
		bool isChildGroup = false;

		float panelAlpha = 0.45f; // only for panel
		float panelRounding = 6.0f; // only for panel


		std::vector<std::string> dropdownOptions;
		int* dropdownCurrentIndex = nullptr;

	};

	class UIManager
	{
	private:
		void RenderElement(UIElement* element);
		UI* m_ui = nullptr;

		std::unordered_map<std::string, std::vector<char>> m_inputBuffers;
		std::unordered_map<std::string, std::string> m_committedText;

		std::vector<std::string> m_frameText; // keeps label/button text alive for this frame

	public:
		Fontloader fontLoader;
		std::vector<UIElement*> GetElements()
		{
			return elements;
		}

		std::vector<UIElement*> elements;

		void setUIRenderer(UI* ui)
		{

			m_ui = ui;
		}
		//void UIManager::BeginGroup(const std::string& groupName);

		//void UIManager::EndGroup();

		void UIManager::BeginPanel(const std::string& panelName);

		void UIManager::EndPanel();

		void UIManager::AddChildToPanel(UIElement* element);

		// Add for new UITypes below

		void AddLabel(int x, int y, float xSize, float ySize, const char* text, const std::string& fontName = "");

		void AddButton(int x, int y, float xSize, float ySize, const char* text);

		void AddCheckbox(int x, int y, float xSize, float ySize, const char* text, bool* checked, const std::string& fontName = "");

		void AddSliderFloat(int x, int y, float xSize, float ySize, const char* text, float* value, float min, float max, const std::string& fontName = "");

		void AddInputText(int xPos, int yPos, float width, const char* label, const char* id, size_t maxLen = 16);

		void AddRadioButton(int x, int y, float xSize, float ySize, const char* text, int* radioValuePointer, int value, bool sameline = false, const std::string& fontName = "");

		void AddDropdown(int x, int y, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex, const std::string& fontName = "");

		void AddColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color, const std::string& fontName = "");

		void ClearElements();

		void AddProgressBar(int x, int y, float xSize, float ySize, float maxvalue, float* currentvalue, float incrementamount, const std::string& fontName = "");

		void AddPanel(int x, int y, float w, float h, float alpha = 0.45f, float rounding = 6.0f);

		void RenderUI();

		std::string GetCommittedText(const std::string& id) const;
		std::unordered_map<std::string, bool> m_inputCommittedThisFrame;
		bool WasInputCommitted(const std::string& id) const;

		void ClearInput(const std::string& id);
	};
}