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
		Toggle,
		BeginChild,
		EndChild
	};

	struct UIElement
	{
		UIType type;

		int xPos;
		int yPos;
		float xSize;
		float ySize;

		std::string ownedText;
		const char* text = nullptr;

		bool sameline = false;
		
		std::string widgetId; // Shared ID for id-based widgets (button/checkbox/slider/input/toggle/dropdown/color)

		bool defaultValue = false; // for checkbox default

		// Slider specific
		float* sliderValue = nullptr; 
		float sliderMin = 0.0f;
		float slidermax = 1.0f;

		// InputText specific
		char* inputBuffer = nullptr; 
		size_t inputBufferSize = 0; 
		float width = 100.0f; 
		std::string inputId;   

		// ProgressBar specific
		float maxValue = 0.0f; 
		float* currentValue = nullptr; 
		float incrementAmount = 0.0f; 

		// Panel specific
		float panelAlpha = 0.45f;
		float panelRounding = 6.0f; 
		int panelR = 0, panelG = 0, panelB = 0; 

		std::vector<UIElement> children;
		bool isChildGroup = false;

		// Centered label specific
		bool centerAligned = false;
		float centerX = 0.0f;
	
		// Radio button specific
	    std::string groupId; 
		int radioValue = 0;           // option value
		int radioDefaultValue = 0; // default selected value

		// Dropdown specific
		std::vector<std::string> dropdownOptions;
		std::string dropdownId;

		// ColorPicker specific
		ImVec4 colorDefault = ImVec4(1, 1, 1, 1);


		// Begin/End child specific
		std::string childId;
		bool childBorder = false;
		ImGuiWindowFlags childFlags = 0;
		bool  childHasBg = false;
		float childBgAlpha = 0.45f;
		float childBgRounding = 6.0f;
		int   childBgR = 0, childBgG = 0, childBgB = 0;

		// Font controls per element
		std::string fontName = "";
		float fontScale = 1.0f;

	};

	class UIManager
	{
	private:
		void RenderElement(UIElement* element);
		UI* m_ui = nullptr;

		std::unordered_map<std::string, std::vector<char>> m_inputBuffers;
		std::unordered_map<std::string, std::string> m_committedText;
		std::unordered_map<std::string, bool> m_inputCommittedThisFrame;

		std::unordered_map<std::string, bool> m_buttonPressed;

		std::unordered_map<std::string, bool> m_checkboxValues;
		std::unordered_map<std::string, bool> m_checkboxChanged;

		std::unordered_map<std::string, float> m_sliderValues;
		std::unordered_map<std::string, bool>  m_sliderChanged;

		std::unordered_map<std::string, bool> m_toggleValues;
		std::unordered_map<std::string, bool> m_toggleChanged;

		std::unordered_map<std::string, int>  m_radioGroupValue;
		std::unordered_map<std::string, bool> m_radioChanged;

		std::unordered_map<std::string, int>  m_dropdownIndex;
		std::unordered_map<std::string, bool> m_dropdownChanged;

		std::unordered_map<std::string, ImVec4> m_colorValues;
		std::unordered_map<std::string, bool>   m_colorChanged;

		struct ChildStylePop { int colors = 0; int vars = 0; };
		std::vector<ChildStylePop> m_childStyleStack;

	public:
		Fontloader fontLoader;
		std::vector<UIElement*> elements;

		std::vector<UIElement*> GetElements() { return elements; }



		void setUIRenderer(UI* ui) { m_ui = ui; }

		void AddChildToPanel(UIElement* element);

		// UI widgets

		void AddLabel(int x, int y, float xSize, float ySize, const char* text, const std::string& fontName = "", float fontScale = 1.0f);

		void AddCenteredLabel(float centerX, float y, const char* text, const std::string& fontName = "", float fontScale = 1.0f);

		void AddButton(int x, int y, float xSize, float ySize, const char* text, const char* id = nullptr,
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddCheckbox(int x, int y, float xSize, float ySize, const char* text, const char* id, bool defaultValue = false,
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddSlider(int x, int y, float width,
			const char* label, const char* id,
			float min, float max, float defaultValue = 0.0f,
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddInputText(int xPos, int yPos, float width, const char* label, const char* id, size_t maxLen = 16,
			const std::string& fontName = "", float fontScale = 1.0f);

		void AddRadioToggle(int x, int y, float xSize, float ySize, const char* label,
			const char* groupId, int value, int defaultValue = 0, bool sameline = false,
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddDropdown(int x, int y, float xSize, float ySize, const char* label,
			const char* id, const std::vector<std::string>& options, int defaultIndex = 0,
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddColorPicker(int x, int y, float xSize, float ySize, const char* label,
			const char* id, ImVec4 defaultColor = ImVec4(1, 1, 1, 1),
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddProgressBar(int x, int y, float xSize, float ySize, float maxvalue, float* currentvalue, float incrementamount, const std::string& fontName = "");

		void AddPanel(int x, int y, float w, float h, float alpha = 0.45f, float rounding = 6.0f,
			int r = 0, int g = 0, int b = 0);


		// State query functions

		bool WasButtonPressed(const std::string& id);

		bool GetCheckbox(const std::string& id) const;
		bool WasCheckboxChanged(const std::string& id);

		float GetSlider(const std::string& id) const;
		bool WasSliderChanged(const std::string& id);
		void SetSlider(const std::string& id, float v); // optional but handy

		std::string GetCommittedText(const std::string& id) const; // For InputText
		bool WasInputCommitted(const std::string& id) const; // For InputText
		void ClearInput(const std::string& id); // For InputText

		int  GetRadio(const std::string& groupId) const;
		bool WasRadioChanged(const std::string& groupId);

		int  GetDropdownIndex(const std::string& id) const;
		bool WasDropdownChanged(const std::string& id);

		ImVec4 GetColor(const std::string& id) const;
		bool   WasColorChanged(const std::string& id);


		// Child windows
		void BeginChild(int x, int y, float w, float h, const char* id,
			bool border = false, ImGuiWindowFlags flags = 0,
			bool hasBg = false, float alpha = 0.45f, float rounding = 6.0f,
			int r = 0, int g = 0, int b = 0);

		void EndChild();

		// Main render functions
		void RenderUI();
		void ClearElements();
	};
}