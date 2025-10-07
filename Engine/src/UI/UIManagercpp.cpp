#include <imgui.h>
#include UiManager.h

using namespace IonixEngine;

void UIManager::CreateLabel(const std::string& text, const ImVec2& position, float fontSize, ImFont* font)
{
	UILabel label;
	label.text = text;
	label.position = position;
	label.fontSize = fontSize;
	label.font = font;
	mlabels.push_back(label);
}

void UIManager::CreateButton(const std::string& text, const ImVec2& position, const ImVec2& size, std::function<void()> onClick, float fontSize, ImFont* font)
{
	UIButton button;
	button.text = text;
	button.position = position;
	button.size = size;
	button.onClick = onClick;
	button.fontSize = fontSize;
	button.font = font;
	mButtons.push_back(button);
}

void 