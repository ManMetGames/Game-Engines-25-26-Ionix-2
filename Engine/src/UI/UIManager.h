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
		Button
	};
	struct UIElement
	{
		UIType type;
		int xPos;
		int yPos;
		char* text;
		std::function<void()> onClick; // only for buttons
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
				{
					e.onClick();
				}
				break;
			}
		}

	}
    
    class UIManager
    {
    public:
		

	private:
		
    };
}

