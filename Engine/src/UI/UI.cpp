#include "UI.h"
#include "imgui.h"
#include "FontLoader.h"
#include "Architecture/Application.h"

namespace IonixEngine
{
	void UI::DrawLabel(char* text, int xsize, int ysize, int xpos, int ypos ,const char* font)
	{
		
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		
	    ImFont* fontToPush = Application::Get().layerUI->m_FontLoader->GetFont("Font1Bold");
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
}
