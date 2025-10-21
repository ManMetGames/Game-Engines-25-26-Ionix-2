#include "UI.h"
#include "imgui.h"
#include <iostream>

namespace IonixEngine
{
	void UI::DrawLabel(char* text, int xpos, int ypos)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetCursorPos(ImVec2(xpos, ypos));

		ImGui::Text(text, ImVec2(xpos, ypos));

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
}
