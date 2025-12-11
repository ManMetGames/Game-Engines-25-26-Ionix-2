#include "UI.h"
#include "imgui.h"
#include "FontLoader.h"
#include "Architecture/Application.h"
#include <stdio.h>
#include <iostream>
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))


namespace IonixEngine
{
	void UI::DrawLabel(char* text, int xsize, int ysize, int xpos, int ypos, std::string font)
	{
		ImGui::SetNextWindowPos(ImVec2((float)xpos, (float)ypos));
		ImGui::SetNextWindowSize(ImVec2((float)xsize, (float)ysize));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;
		char windowName[64];
		std::snprintf(windowName, sizeof(windowName), "Label_%d_%d_%d_%d", xpos, ypos, xsize, ysize);
		if (ImGui::Begin(windowName, nullptr, flags))
		{
			ImFont* fontToPush = nullptr;
			if (Application::Get().layerUI && Application::Get().layerUI->m_FontLoader)
			{
				fontToPush = Application::Get().layerUI->m_FontLoader->GetFont("Font1Bold");
			}
			if (fontToPush)
			{
				ImGui::PushFont(fontToPush);
				ImGui::TextUnformatted(text);
				ImGui::PopFont();
			}
			else
			{
				ImGui::TextUnformatted(text);
			}
		}
		ImGui::End();
	}

	bool UI::DrawButton(char* text, int xsize, int ysize, int xpos, int ypos)
	{
		ImGui::SetCursorPos(ImVec2(xpos, ypos));

		if (ImGui::Button(text, ImVec2(xsize, ysize))) {
			return true;
		}
	}

	float UI::DrawSlider(char* text, static float i, int xsize, int ysize, int xpos, int ypos, int minval, int maxval) {
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		ImGui::SliderFloat(text, &i, minval, maxval);
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

	void UI::DrawRadioButton(int xpos, int ypos, char* text, int& e, int value, bool sameline)
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
	
	void UI::DrawProgressBar(int xPos, int yPos, float xSize, float ySize, float maxValue, float currentValue, int colorId)
	{
		float clampedMax = maxValue <= 0.0f ? 1.0f : maxValue;
		float clampedCurrent = currentValue;
		if (clampedCurrent < 0.0f) clampedCurrent = 0.0f;
		if (clampedCurrent > clampedMax) clampedCurrent = clampedMax;
		float progress = clampedCurrent / clampedMax;
		progress = IM_CLAMP(progress, 0.0f, 1.0f);
		ImGui::SetNextWindowPos(ImVec2((float)xPos, (float)yPos));
		ImGui::SetNextWindowSize(ImVec2(xSize, ySize));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;
		char windowName[64];
		std::snprintf(windowName, sizeof(windowName), "Bar_%d_%d_%d_%d", xPos, yPos, (int)xSize, (int)ySize);
		if (ImGui::Begin(windowName, nullptr, flags))
		{
			ImVec4 fgColor;
			ImVec4 bgColor;
			switch (colorId)
			{
			case 1: // red
				fgColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
				bgColor = ImVec4(0.2f, 0.0f, 0.0f, 0.6f);
				break;
			case 2: // green
				fgColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
				bgColor = ImVec4(0.0f, 0.2f, 0.0f, 0.6f);
				break;
			case 3: // white
				fgColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				bgColor = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);
				break;
			default: // 0 = default/orange
				fgColor = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
				bgColor = ImVec4(0.2f, 0.12f, 0.0f, 0.6f);
				break;
			}
			ImGui::PushStyleColor(ImGuiCol_PlotHistogram, fgColor);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor);
			ImGui::ProgressBar(progress, ImVec2(xSize, ySize), "");
			ImGui::PopStyleColor(2);
		}
		ImGui::End();
	}
	
	float UI::ProgressBar(int xPos, int yPos, float xSize, float ySize, float maxValue, float& currentValue, float incrementAmount)
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

	float UI::DrawDropdown(int xPos, int yPos, float xSize, float ySize, const char* text, std::vector<std::string> options, int* currentIndex)
	{
		ImGui::SetCursorPos(ImVec2(xPos, yPos));

		const char* currentItem = options[*currentIndex].c_str();
		if (ImGui::BeginCombo(text, currentItem))
		{
			for (size_t n = 0; n < options.size(); n++)
			{
				bool isSelected = (*currentIndex == n);
				if (ImGui::Selectable(options[n].c_str(), isSelected))
				{
					*(currentIndex) = n;
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		return *currentIndex;
	}
}