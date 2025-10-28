#include "Input.h"
namespace IonixEngine 
{


	MouseCoords IonixEngine::Input::GetMousePosition()
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		MouseCoords mc(x, y);
		return mc; 
	}
	bool Input::IsKeyDown(SDL_Scancode code) const
	{
		// Not held down previous frame

		// Held down current frame
		return !previousKeys.count(code) && currentKeys.count(code);
	}
	bool Input::IsKeyUp(SDL_Scancode code) const
	{
		// Was held down previous frame

	   // No longer held down on current frame
		return previousKeys.count(code) && !currentKeys.count(code);
	}
	bool Input::IsKeyHeld(SDL_Scancode code) const
	{
		return currentKeys.count(code);
	}
	bool Input::IsMouseButtonDown(Uint8 mousecode) const
	{
		return currentMouse.count(mousecode);
	}

	bool Input::IsMouseButtonUp(Uint8 mousecode) const
	{
		return previousMouse.count(mousecode);
	}

	void Input::SetKeyPressed(SDL_Scancode code)
	{
		currentKeys.insert(code);
	}

	void Input::SetKeyReleased(SDL_Scancode code)
	{
		currentKeys.erase(code);
	}

	void Input::SetMousePressed(Uint8 code)
	{
		currentMouse.insert(code);
	}

	void Input::SetMouseReleased(Uint8 code)
	{
		currentMouse.erase(code);
	}

	float Input::ScrollWheel(Uint8 code)const
	{
		return currentScroll.count(code);
	}

	void Input::CopyCodesEndFrame()
	{
		previousKeys = currentKeys;
		previousMouse = currentMouse;
		previousScroll = currentScroll;
	}	

	
}

