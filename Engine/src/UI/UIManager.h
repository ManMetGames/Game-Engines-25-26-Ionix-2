#pragma once
#include <imgui.h>
#include <string>
#include <functional>
#include <vector>
namespace IonixEngine
{
	enum UIType
	{
		Label,
		Button,
		Checkbox,
		SliderFloat,
		InputText,
	};
	struct UIElement
	{
		UIType type;
		int xPos;
		int yPos;
		char* text;
		std::function<void()> onClick; // only for buttons
		bool* checked; // only for checkboxes
		float* sliderValue; // only for sliders
		float sliderMin = 0.0f;// only for sliders
		float slidermax = 1.0f;// only for sliders
		char* inputBuffer; // only for input text
		size_t inputBufferSize; // only for input text
	};

	std::vector<UIElement> elements;
	// end user, all they need is a function to push a new UI element to this vector
	void AddLabel(int x, int y, const char* text)
		{
			elements.push_back({ UIType::Label, x, y, const_cast<char*>(text), nullptr });
		}
	void AddButton(int x, int y, const char* text, std::function<void()> onClick)
	{
		elements.push_back({ UIType::Button, x, y, const_cast<char*>(text), onClick });
	}
	void AddCheckbox(int x, int y, const char* text, bool* checked)
	{
		elements.push_back({ UIType::Checkbox, x, y, const_cast<char*>(text), nullptr, checked });
	}
	void AddSliderFloat(int x, int y, const char* text, float* value, float min, float max)
	{
		elements.push_back({ UIType::SliderFloat, x, y, const_cast<char*>(text), nullptr, nullptr, value, min, max });
	}
	void AddInputText(int x, int y, const char* text, char* buffer, size_t bufferSize)
	{
		elements.push_back({ UIType::InputText, x, y, const_cast<char*>(text), nullptr, nullptr, nullptr, 0.0f, 0.0f, buffer, bufferSize });
	}

    void RenderUI()
    {
       for (auto& e : elements)
       {
           ImGui::SetCursorPos(ImVec2((float)e.xPos, (float)e.yPos));

           switch (e.type)
           {
           case UIType::Label:
               ImGui::Text("%s", e.text);
               break;
           case UIType::Button:
               if (ImGui::Button(e.text) && e.onClick)
                   e.onClick();
               break; // Added break to prevent fallthrough
           case UIType::Checkbox:
               if (e.checked)
                   ImGui::Checkbox(e.text, e.checked);
               break; // Added break to prevent fallthrough
           case UIType::SliderFloat:
               if (e.sliderValue)
                   ImGui::SliderFloat(e.text, e.sliderValue, e.sliderMin, e.slidermax);
               break; // Added break to prevent fallthrough
           case UIType::InputText:
               if (e.inputBuffer && e.inputBufferSize > 0)
                   ImGui::InputText(e.text, e.inputBuffer, e.inputBufferSize);
               break;
           }
       }
    }
    
    
}

