#include "UI.h"
#include "imgui.h"
#include "FontLoader.h"
#include "Architecture/Application.h"
#include <stdio.h>
#include <iostream>
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))


namespace IonixEngine
{
	void UI::DrawLabel(const char* text, int xsize, int ysize, int xpos, int ypos, float wrapWidth)
	{
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		if (wrapWidth > 0.0f)
		{
			ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + wrapWidth);
			ImGui::TextUnformatted(text);
			ImGui::PopTextWrapPos();
		}
		else
		{
			ImGui::TextUnformatted(text);
		}
	}

	bool UI::DrawButton(char* text, int xsize, int ysize, int xpos, int ypos)
	{
		ImGui::SetCursorPos(ImVec2(xpos, ypos));
		return ImGui::Button(text, ImVec2(xsize, ysize));
	}

	bool UI::DrawSlider(const char* label, float* value, float width,
		int xpos, int ypos, float minval, float maxval, const char* format)
	{
		ImGui::SetCursorPos(ImVec2((float)xpos, (float)ypos));
		ImGui::SetNextItemWidth(width);
		return ImGui::SliderFloat(label, value, minval, maxval, format);
	}

	bool UI::DrawCheckbox(const char* label, bool* value, int xpos, int ypos)
	{
		ImGui::SetCursorPos(ImVec2((float)xpos, (float)ypos));
		return ImGui::Checkbox(label, value);
	}

	bool UI::DrawRadioButton(int xpos, int ypos, const char* text, int* v, int value)
	{
		ImGui::SetCursorPos(ImVec2((float)xpos, (float)ypos));
		return ImGui::RadioButton(text, v, value);
	}

	bool UI::DrawColorPicker(int x, int y, float xSize, float ySize, const char* label, float* color4)
	{
		ImGui::SetCursorPos(ImVec2((float)x, (float)y));
		if (xSize > 0.0f) ImGui::SetNextItemWidth(xSize);
		return ImGui::ColorEdit4(label, color4,
			ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_PickerHueWheel);
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

	void UI::DrawProgressBarStyled(int xPos, int yPos, float xSize, float ySize,
		float maxValue, float currentValue, int colorId,
		float rounding, float borderSize,
		bool useColors, ImVec4 bg, ImVec4 fill, ImVec4 border,
		const char* overlayText)
	{
		float clampedMax = (maxValue <= 0.0f) ? 1.0f : maxValue;
		float clampedCurrent = currentValue;
		if (clampedCurrent < 0.0f) clampedCurrent = 0.0f;
		if (clampedCurrent > clampedMax) clampedCurrent = clampedMax;

		float progress = clampedCurrent / clampedMax;
		progress = IM_CLAMP(progress, 0.0f, 1.0f);

		ImVec4 fgColor;
		ImVec4 bgColor;
		ImVec4 borderColor = ImVec4(0, 0, 0, 0);

		if (useColors)
		{
			fgColor = fill;
			bgColor = bg;
			borderColor = border;
		}
		else
		{
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
		}

		ImGui::SetNextWindowPos(ImVec2((float)xPos, (float)yPos));
		ImGui::SetNextWindowSize(ImVec2(xSize, ySize));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

		char windowName[64];
		std::snprintf(windowName, sizeof(windowName), "BarStyled_%d_%d_%d_%d", xPos, yPos, (int)xSize, (int)ySize);

		// Make the bar fill the whole window (no padding/spacing)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		int pushedVars = 2;

		if (rounding >= 0.0f) { ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding); pushedVars++; }
		if (borderSize >= 0.0f) { ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderSize); pushedVars++; }

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, fgColor);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, bgColor);
		int pushedColors = 2;

		if (borderSize > 0.0f)
		{
			ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
			pushedColors++;
		}

		if (ImGui::Begin(windowName, nullptr, flags))
		{
			const char* overlay = (overlayText && overlayText[0]) ? overlayText : "";
			ImGui::ProgressBar(progress, ImVec2(xSize, ySize), overlay);
		}
		ImGui::End();

		ImGui::PopStyleColor(pushedColors);
		ImGui::PopStyleVar(pushedVars);
	}

	void UI::DrawProgressBarStyledCapped(int xPos, int yPos, float xSize, float ySize,
		float maxValue, float currentValue, int colorId,
		float rounding, float borderSize,
		bool useColors, ImVec4 bg, ImVec4 fill, ImVec4 border,
		float capMax, ImVec4 capFill,
		const char* overlayText)
	{
		// If cap isn't valid, fall back to the normal styled bar (backwards compatible)
		if (capMax <= 0.0f || capMax >= maxValue || capFill.w <= 0.0f)
		{
			DrawProgressBarStyled(xPos, yPos, xSize, ySize,
				maxValue, currentValue, colorId,
				rounding, borderSize,
				useColors, bg, fill, border,
				overlayText);
			return;
		}

		float clampedMax = (maxValue <= 0.0f) ? 1.0f : maxValue;
		if (capMax < 0.0f) capMax = 0.0f;
		if (capMax > clampedMax) capMax = clampedMax;

		float capCurrent = currentValue;
		if (capCurrent < 0.0f) capCurrent = 0.0f;
		if (capCurrent > capMax) capCurrent = capMax;

		// fracCap = portion of the bar that remains "available"
		float fracCap = capMax / clampedMax;
		fracCap = IM_CLAMP(fracCap, 0.0f, 1.0f);

		// current as a fraction of cap, then mapped into full bar width
		float fracFillInCap = (capMax > 0.0f) ? (capCurrent / capMax) : 0.0f;
		fracFillInCap = IM_CLAMP(fracFillInCap, 0.0f, 1.0f);

		float fracCur = fracCap * fracFillInCap; // fill fraction in whole bar
		fracCur = IM_CLAMP(fracCur, 0.0f, 1.0f);

		ImVec4 fgColor;
		ImVec4 bgColor;
		ImVec4 borderColor = ImVec4(0, 0, 0, 0);

		if (useColors)
		{
			fgColor = fill;
			bgColor = bg;
			borderColor = border;
		}
		else
		{
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
		}

		ImGui::SetNextWindowPos(ImVec2((float)xPos, (float)yPos));
		ImGui::SetNextWindowSize(ImVec2(xSize, ySize));
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

		char windowName[64];
		std::snprintf(windowName, sizeof(windowName), "BarStyledCap_%d_%d_%d_%d", xPos, yPos, (int)xSize, (int)ySize);

		// No padding/spacing for a clean draw-list based bar
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		int pushedVars = 2;

		float usedRounding = (rounding >= 0.0f) ? rounding : ImGui::GetStyle().FrameRounding;
		float thick = (borderSize > 0.0f) ? borderSize : 0.0f;

		if (ImGui::Begin(windowName, nullptr, flags))
		{
			ImDrawList* dl = ImGui::GetWindowDrawList();
			ImVec2 p0 = ImGui::GetWindowPos();
			ImVec2 p1 = ImVec2(p0.x + xSize, p0.y + ySize);
			float w = xSize;

			// Background (full bar)
			dl->AddRectFilled(p0, p1, ImGui::GetColorU32(bgColor), usedRounding, ImDrawFlags_RoundCornersAll);

			// Fill (left side, up to fracCur)
			if (fracCur > 0.0f)
			{
				ImVec2 f1 = ImVec2(p0.x + w * fracCur, p1.y);
				ImDrawFlags fFlags = (fracCur >= 1.0f) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersLeft;
				dl->AddRectFilled(p0, f1, ImGui::GetColorU32(fgColor), usedRounding, fFlags);
			}

			// "Lost max" cap segment (right side, from fracCap to 1.0)
			if (fracCap < 1.0f)
			{
				ImVec2 c0 = ImVec2(p0.x + w * fracCap, p0.y);
				ImDrawFlags cFlags = ImDrawFlags_RoundCornersRight;
				dl->AddRectFilled(c0, p1, ImGui::GetColorU32(capFill), usedRounding, cFlags);
			}

			// Border
			if (thick > 0.0f && borderColor.w > 0.0f)
			{
				dl->AddRect(p0, p1, ImGui::GetColorU32(borderColor), usedRounding, ImDrawFlags_RoundCornersAll, thick);
			}

			// Overlay text centered
			const char* overlay = (overlayText && overlayText[0]) ? overlayText : "";
			if (overlay[0] != '\0')
			{
				ImVec2 ts = ImGui::CalcTextSize(overlay);
				ImVec2 tp = ImVec2(p0.x + (xSize - ts.x) * 0.5f, p0.y + (ySize - ts.y) * 0.5f);
				dl->AddText(tp, ImGui::GetColorU32(ImGuiCol_Text), overlay);
			}
		}
		ImGui::End();

		ImGui::PopStyleVar(pushedVars);
	}

	bool UI::DrawDropdown(int xPos, int yPos, float xSize, float ySize, const char* text,
		const std::vector<std::string>& options, int* currentIndex)
	{
		ImGui::SetCursorPos(ImVec2((float)xPos, (float)yPos));
		if (xSize > 0.0f) ImGui::SetNextItemWidth(xSize);

		// Use ySize as a desired control height (by adjusting FramePadding.y)
		bool pushedPadding = false;
		if (ySize > 0.0f)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			float padY = (ySize - ImGui::GetFontSize()) * 0.5f;
			if (padY < 0.0f) padY = 0.0f;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, padY));
			pushedPadding = true;
		}

		if (!currentIndex)
		{
			if (pushedPadding) ImGui::PopStyleVar();
			return false;
		}

		if (options.empty())
		{
			bool opened = ImGui::BeginCombo(text, "(empty)");
			if (opened) ImGui::EndCombo();
			if (pushedPadding) ImGui::PopStyleVar();
			return false;
		}

		if (*currentIndex < 0 || *currentIndex >= (int)options.size())
			*currentIndex = 0;

		bool changed = false;
		const char* currentItem = options[*currentIndex].c_str();

		if (ImGui::BeginCombo(text, currentItem))
		{
			for (int n = 0; n < (int)options.size(); n++)
			{
				bool isSelected = (*currentIndex == n);
				if (ImGui::Selectable(options[n].c_str(), isSelected))
				{
					*currentIndex = n;
					changed = true;
				}
				if (isSelected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		if (pushedPadding) ImGui::PopStyleVar();
		return changed;
	}

	bool UI::InputText(const char* label, int xPos, int yPos, float width,
		char* buffer, size_t bufferSize,
		ImGuiInputTextFlags flags)
	{

		ImGui::SetCursorPos(ImVec2((float)xPos, (float)yPos));
		ImGui::TextUnformatted(label);

		ImGui::SetCursorPos(ImVec2((float)xPos, (float)yPos + 20));
		ImGui::SetNextItemWidth(width);

		return ImGui::InputText("##input", buffer, bufferSize, flags);
	}

	void UI::DrawPanel(int x, int y, float w, float h, float alpha, float rounding, int r, int g, int b)
	{
		ImGui::SetCursorPos(ImVec2((float)x, (float)y));
		ImVec2 p = ImGui::GetCursorScreenPos();

		ImU32 fill = IM_COL32(r, g, b, (int)(alpha * 255.0f));
		ImDrawList* dl = ImGui::GetWindowDrawList();

		dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), fill, rounding);

		ImU32 border = IM_COL32(0, 0, 0, 80); // border for panel
		dl->AddRect(p, ImVec2(p.x + w, p.y + h), border, rounding);
	}

	float UI::CalcTextWidth(const char* text)
	{
		return ImGui::CalcTextSize(text).x;
	}
}