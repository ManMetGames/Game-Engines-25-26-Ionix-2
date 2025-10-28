#include "UI.h"
#include "imgui.h"
#include <stdio.h>
#include <iostream>

#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

namespace IonixEngine
{
	void UI::DrawLabel(char* text, int xsize, int ysize, int xpos, int ypos)
	{
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		ImGui::Text(text, ImVec2(xsize, ysize));
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

	/*void UI::DrawRadioButton(int xpos, int ypos, char* text, static int e, int value, bool sameline)
	{
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		if (sameline == true)
		{
			ImGui::RadioButton(text, &e, value); ImGui::SameLine();
		}
		ImGui::RadioButton(text, &e, value);
	}*/

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

}
