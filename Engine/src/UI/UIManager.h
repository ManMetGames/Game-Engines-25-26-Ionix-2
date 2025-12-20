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
		Slider,
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

		// Button specific
		bool  buttonCustomStyle = false;
		float buttonRounding = 0.0f;
		ImVec4 buttonColor = ImVec4(0, 0, 0, 0);

		bool defaultValue = false; // for checkbox default

		// Slider specific
		float* sliderValue = nullptr;
		float sliderMin = 0.0f;
		float slidermax = 1.0f;
		std::string sliderFormat;

		// Slider style (optional)
		bool  sliderCustomStyle = false;

		float sliderHeightPx = 0.0f;      // thickness/height in pixels (0 = default)
		float sliderRounding = -1.0f;     // -1 = default
		float sliderGrabRounding = -1.0f; // -1 = use sliderRounding
		float sliderGrabMinSize = 0.0f;   // 0 = default

		bool  sliderUseColors = false;
		ImVec4 sliderTrack = ImVec4(0, 0, 0, 0);
		ImVec4 sliderGrab = ImVec4(0, 0, 0, 0);

		// InputText specific
		char* inputBuffer = nullptr;
		size_t inputBufferSize = 0;
		float width = 100.0f;
		std::string inputId;

		// ProgressBar specific
		float maxValue = 0.0f;
		float* currentValue = nullptr;
		float incrementAmount = 0.0f;


		// Value-mode (non-mutating) progress bar (used by scripting)
		float progressCurrentValue = 0.0f;
		int   progressColorId = 0;

		// Progress bar style (optional)
		bool  progressCustomStyle = false;
		float progressRounding = -1.0f;      // -1 = default
		float progressBorderSize = -1.0f;    // -1 = default
		bool  progressUseColors = false;
		ImVec4 progressBg = ImVec4(0, 0, 0, 0);
		ImVec4 progressFill = ImVec4(0, 0, 0, 0);
		ImVec4 progressBorder = ImVec4(0, 0, 0, 0);
		std::string progressOverlayText;

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

		// Dropdown style (optional)
		bool  dropdownCustomStyle = false;

		float dropdownRounding = -1.0f;       // -1 = default
		float dropdownPopupRounding = -1.0f;  // -1 = default
		float dropdownBorderSize = -1.0f;     // -1 = default

		bool  ddHasFrame = false;
		ImVec4 ddFrame = ImVec4(0, 0, 0, 0);
		bool  ddHasFrameHovered = false;
		ImVec4 ddFrameHovered = ImVec4(0, 0, 0, 0);
		bool  ddHasFrameActive = false;
		ImVec4 ddFrameActive = ImVec4(0, 0, 0, 0);

		bool  ddHasPopupBg = false;
		ImVec4 ddPopupBg = ImVec4(0, 0, 0, 0);

		bool  ddHasBorder = false;
		ImVec4 ddBorder = ImVec4(0, 0, 0, 0);

		// Selectable colors inside the dropdown list
		bool  ddHasItem = false;
		ImVec4 ddItem = ImVec4(0, 0, 0, 0);
		bool  ddHasItemHovered = false;
		ImVec4 ddItemHovered = ImVec4(0, 0, 0, 0);
		bool  ddHasItemActive = false;
		ImVec4 ddItemActive = ImVec4(0, 0, 0, 0);

		bool  ddHasText = false;
		ImVec4 ddText = ImVec4(0, 0, 0, 0);

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
		float childBorderSize = 1.0f;     // thickness
		bool  childAutoBorderColor = true;
		float childBorderDarken = 0.85f;  // 0.85 = slightly darker than bg
		ImVec4 childBorderColor = ImVec4(0, 0, 0, 1); // only used if auto=false

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

		void AddButton(int x, int y, float w, float h, const char* text, const char* id = nullptr,
			const std::string& fontName = "", float fontScale = 1.0f,
			float rounding = 0.0f, bool useColor = false,
			float r = 0, float g = 0, float b = 0, float a = 1.0f);


		void AddCheckbox(int x, int y, float xSize, float ySize, const char* text, const char* id, bool defaultValue = false,
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddSlider(int x, int y, float width,
			const char* label, const char* id,
			float min, float max, float defaultValue = 0.0f,
			const std::string& fontName = "", float fontScale = 1.0f, const std::string& format = "");

		void AddSliderStyled(int x, int y, float width,
			const char* label, const char* id,
			float min, float max, float defaultValue = 0.0f,
			const std::string& fontName = "", float fontScale = 1.0f,
			const std::string& format = "",
			float heightPx = 0.0f, float rounding = -1.0f, float grabSize = 0.0f,
			bool useColors = false, ImVec4 track = ImVec4(0, 0, 0, 0), ImVec4 grab = ImVec4(0, 0, 0, 0));


		void AddInputText(int xPos, int yPos, float width, const char* label, const char* id, size_t maxLen = 16,
			const std::string& fontName = "", float fontScale = 1.0f);

		void AddRadioToggle(int x, int y, float xSize, float ySize, const char* label,
			const char* groupId, int value, int defaultValue = 0, bool sameline = false,
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddDropdown(int x, int y, float xSize, float ySize, const char* label,
			const char* id, const std::vector<std::string>& options, int defaultIndex = 0,
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddDropdownStyled(int x, int y, float xSize, float ySize, const char* label,
			const char* id, const std::vector<std::string>& options, int defaultIndex = 0,
			const std::string& fontName = "", float fontScale = 1.0f,
			float heightPx = 0.0f,
			float rounding = -1.0f, float popupRounding = -1.0f, float borderSize = -1.0f,
			bool hasFrame = false, ImVec4 frame = ImVec4(0, 0, 0, 0),
			bool hasFrameHovered = false, ImVec4 frameHovered = ImVec4(0, 0, 0, 0),
			bool hasFrameActive = false, ImVec4 frameActive = ImVec4(0, 0, 0, 0),
			bool hasPopupBg = false, ImVec4 popupBg = ImVec4(0, 0, 0, 0),
			bool hasBorder = false, ImVec4 borderCol = ImVec4(0, 0, 0, 0),
			bool hasItem = false, ImVec4 item = ImVec4(0, 0, 0, 0),
			bool hasItemHovered = false, ImVec4 itemHovered = ImVec4(0, 0, 0, 0),
			bool hasItemActive = false, ImVec4 itemActive = ImVec4(0, 0, 0, 0),
			bool hasText = false, ImVec4 textCol = ImVec4(0, 0, 0, 0));


		void AddColorPicker(int x, int y, float xSize, float ySize, const char* label,
			const char* id, ImVec4 defaultColor = ImVec4(1, 1, 1, 1),
			const std::string& fontName = "", float fontScale = 1.0f);


		void AddProgressBar(int x, int y, float xSize, float ySize, float maxvalue, float* currentvalue, float incrementamount, const std::string& fontName = "");


		// Draw-only progress bar (does NOT mutate value) + supports colorId like UI::DrawProgressBar
		void AddProgressBarValue(int x, int y, float xSize, float ySize,
			float maxValue, float currentValue, int colorId = 0,
			const std::string& fontName = "", float fontScale = 1.0f);

		// Styled draw-only progress bar (rounded, custom colors)
		void AddProgressBarValueStyled(int x, int y, float xSize, float ySize,
			float maxValue, float currentValue, int colorId,
			float rounding, float borderSize,
			bool useColors, ImVec4 bg, ImVec4 fill, ImVec4 border,
			const std::string& overlayText = "",
			const std::string& fontName = "", float fontScale = 1.0f);

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
		std::string GetLiveText(const std::string& id) const; // For InputText (live buffer)
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
			int r = 0, int g = 0, int b = 0, float borderSize = 1.0f,
			bool autoBorder = true, float borderDarken = 0.85f,
			ImVec4 borderColor = ImVec4(0, 0, 0, 1));

		void EndChild();

		// Main render functions
		void RenderUI();
		void ClearElements();
	};
}