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

	void UI::DrawRadioButton(int xpos, int ypos, char* text, static int e, int value, bool sameline)
	{
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		if (sameline == true)
		{
			ImGui::RadioButton(text, &e, value); ImGui::SameLine();
		}
		ImGui::RadioButton(text, &e, value);
	}

	void UI::ProgressBar(int xpos, int ypos, float xsize, float ysize, float maxvalue, float currentvalue, float incrementamount, bool increaseDirection)
	{
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		
		// Calculate progress as a ratio (0.0 to 1.0)
		float progress = currentvalue / maxvalue;
		
		// Clamp progress between 0 and 1
		progress = IM_CLAMP(progress, 0.0f, 1.0f);
		
		// Format display text
		char buf[32];
		sprintf(buf, "%.0f/%.0f", currentvalue, maxvalue);
		
		ImGui::ProgressBar(progress, ImVec2(xsize, ysize), buf);
	}

}
