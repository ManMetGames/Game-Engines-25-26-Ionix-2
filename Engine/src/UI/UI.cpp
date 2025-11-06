#include "UI.h"
#include "imgui.h"
#include "FontLoader.h"
#include "Architecture/Application.h"
#include <stdio.h>
#include <iostream>
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))


namespace IonixEngine
{
	void UI::DrawLabel(char* text, int xsize, int ysize, int xpos, int ypos , std::string font)
	{
		
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		std::unordered_map<std::string, ImFont*>& map = Application::Get().layerUI->GetUIManager().fontLoader.fontMap;
		ImFont* fontToPush = map[font];

		ImGui::PushFont(fontToPush);
		ImGui::Text(text, ImVec2(xsize, ysize));
		ImGui::PopFont();

		/*ImFont* BoldFontPush = Application::Get().layerUI->m_FontLoader->GetFont("FontBold");
		ImGui::PushFont(BoldFontPush);
		ImGui::Text(text, ImVec2(xsize, ysize));
		ImGui::PopFont();*/

	}
	bool UI::DrawButton(char* text, int xsize, int ysize, int xpos, int ypos)
	{
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		
		if (ImGui::Button(text, ImVec2(xsize, ysize)))
			return true;
	}
	
	float UI::DrawSlider(char* text, static float i, int xsize, int ysize, int xpos, int ypos,int minval, int maxval) {
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		ImGui::SliderFloat(text, &i, minval, maxval );
		return i;

	}
	
	void UI::DrawCheckbox(int id, char* text, int xpos, int ypos, int xsize, int ysize)
	{
		if (checkboxMap.find(id) != checkboxMap.end())
			checkboxMap.insert({ id, false });

		bool& state = getCheckboxState(id);
		ImGui::SetCursorPos(ImVec2(xpos, ypos));

		ImGui::Checkbox(text, &state);
	}

	void UI::DrawRadioButton(int xpos, int ypos, char* text, int &e, int value, bool sameline)
	{
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		if (sameline == true)
		{
			ImGui::RadioButton(text, &e, value); ImGui::SameLine();
		}

		else
		{
			ImGui::RadioButton(text, &e, value);
		}
	}
	float UI::DrawColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color)
	{
		ImGui::SetCursorPos(ImVec2(x, y));
		ImGui::ColorEdit4(label, color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueWheel);
		return *color;
	}
	

	float UI::DrawProgressBar(int xPos, int yPos, float xSize, float ySize, float maxValue, float& currentValue, float incrementAmount)
	{
		ImGui::SetCursorPos(ImVec2(xPos, yPos));
		
		
		// Positive = increase, Negative = decrease
		currentValue += incrementAmount;
		

		if (currentValue < 0.0f) currentValue = 0.0f;
		if (currentValue > maxValue) currentValue = maxValue;
		
		float progress = currentValue / maxValue;
		progress = IM_CLAMP(progress, 0.0f, 1.0f);

		char buf[32];
		sprintf(buf, "%.0f/%.0f", currentValue, maxValue);
		
		ImGui::ProgressBar(progress, ImVec2(xSize, ySize), buf);
		
		return currentValue;
	}
}
