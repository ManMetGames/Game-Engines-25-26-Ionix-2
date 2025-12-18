#include "Input.h"
#include <cmath> 

namespace IonixEngine
{
    MouseCoords Input::GetMousePosition()
    {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return MouseCoords(x, y);
    }

    float scrollDiff;

    //Keyboard
    bool Input::IsKeyDown(SDL_Scancode code) const
    {
        return !previousKeys.count(code) && currentKeys.count(code);
    }

    bool Input::IsKeyUp(SDL_Scancode code) const
    {
        return previousKeys.count(code) && !currentKeys.count(code);
    }

    bool Input::IsKeyHeld(SDL_Scancode code) const
    {
        return currentKeys.count(code);
    }

    void Input::SetKeyPressed(SDL_Scancode code)
    {
        currentKeys.insert(code);
    }

    void Input::SetKeyReleased(SDL_Scancode code)
    {
        currentKeys.erase(code);
    }

    void Input::SetScrollDiff(float diff)
    {
        scrollDiff = diff;
    }

    float Input::GetScrollDiff()
    {
        return scrollDiff;
    }


    //Mouse
    bool Input::IsMouseButtonDown(Uint8 mousecode) const
    {
        return !previousMouse.count(mousecode) && currentMouse.count(mousecode);
    }

    bool Input::IsMouseButtonUp(Uint8 mousecode) const
    {
        return previousMouse.count(mousecode) && !currentMouse.count(mousecode);
    }

    void Input::SetMousePressed(Uint8 code)
    {
        currentMouse.insert(code);
    }

    void Input::SetMouseReleased(Uint8 code)
    {
        currentMouse.erase(code);
    }



    //Frame management
    void Input::CopyCodesEndFrame()
    {
        previousKeys = currentKeys;
        previousMouse = currentMouse;

    }
}

// Implementation of BattleshipInputAdapter

namespace IonixEngine
{
    // Initializes adapter with board geometry and resets flags
    BattleshipInputAdapter::BattleshipInputAdapter(Input* input, int boardOriginX, int boardOriginY, int cellSize, int boardWidth, int boardHeight)
        : m_input(input),
        m_originX(boardOriginX),
        m_originY(boardOriginY),
        m_cellSize(cellSize),
        m_boardW(boardWidth),
        m_boardH(boardHeight),
        m_lastClick(-1, -1),
        m_rotate(false),
        m_reset(false)
    {
    }

    // Checks mouse clicks and keyboard shortcuts each frame
    void BattleshipInputAdapter::Update()
    {
        if (m_input->IsMouseButtonDown(SDL_BUTTON_LEFT))
        {
            auto mouse = m_input->GetMousePosition();

            // Convert screen coordinates to board cell
            int cellX = (mouse.x - m_originX) / m_cellSize;
            int cellY = (mouse.y - m_originY) / m_cellSize;

            // Validate cell within board bounds
            if (cellX >= 0 && cellX < m_boardW &&
                cellY >= 0 && cellY < m_boardH)
            {
                m_lastClick = { cellX, cellY };
            }
            else
            {
                m_lastClick = { -1, -1 }; // invalid click outside board
            }
        }

        //Keyboard shortcuts
        // Rotate ship placement when "R" is pressed
        m_rotate = m_input->IsKeyDown(SDL_SCANCODE_R);

        // Reset game when "Escape" is pressed
        m_reset = m_input->IsKeyDown(SDL_SCANCODE_ESCAPE);
    }

    // Returns last valid cell clicked (or {-1,-1} if none)
    std::pair<int, int> BattleshipInputAdapter::GetLastCellClick() const
    {
        return m_lastClick;
    }

    // Returns true if rotate was requested this frame
    bool BattleshipInputAdapter::RotateRequested() const
    {
        return m_rotate;
    }

    // Returns true if reset was requested this frame
    bool BattleshipInputAdapter::ResetRequested() const
    {
        return m_reset;
    }
}