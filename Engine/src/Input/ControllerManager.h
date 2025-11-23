#pragma once
#include <unordered_set>
#include <SDL.h>

namespace IonixEngine
{

    class ControllerManager
    {
    public:

        //Controller buttons
        bool IsButtonDown(Uint8 btn) const;
        bool IsButtonUp(Uint8 btn) const;
        bool IsButtonHeld(Uint8 btn) const;
        void SetButtonPressed(Uint8 btn);
        void SetButtonReleased(Uint8 btn);

        //Controller axis and trigger normalization
        float NormaliseStickAxis(float axis);
        float NormaliseTrigger(float axis);

        //Update state
        void CopyCodesEndFrame();

        //Getters
        float GetXStickHor() { return controllerAxis; }

    private:

        //Controller buttons
        std::unordered_set<Uint8> currentButton;
        std::unordered_set<Uint8> previousButton;

        //Controller axis
        float controllerAxis = 0.0f;
        float triggerPressure = 0.0f;
    };
}

