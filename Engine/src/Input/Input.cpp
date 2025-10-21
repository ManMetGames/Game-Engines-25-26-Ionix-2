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
		return mousecode != 0 ? true : false;
	}

	void Input::SetKeyPressed(SDL_Scancode code)
	{
		currentKeys.insert(code);
	}

	void Input::SetKeyReleased(SDL_Scancode code)
	{

		currentKeys.erase(code);
	}

	void Input::CopyCodesEndFrame()
	{
		previousKeys = currentKeys;
	}


	
}

