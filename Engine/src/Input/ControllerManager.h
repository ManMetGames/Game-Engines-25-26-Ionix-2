#pragma once
#include <unordered_set>
#include <SDL.h>

namespace IonixEngine
{

    class ControllerManager
    {
    public:
        ControllerManager() { }
        ControllerManager(int id) { instanceId = id; }

        //Controller buttons
        bool IsButtonDown(int instanceId, Uint8 btn) const;
        bool IsButtonUp(int instanceId, Uint8 btn) const;
        bool IsButtonHeld(int instanceId, Uint8 btn) const;
        void SetButtonPressed(Uint8 btn);
        void SetButtonReleased(Uint8 btn);

        //Controller axis and trigger normalization
        float NormaliseStickAxis(int instanceId, float axis);
        float NormaliseTrigger(int instanceId, float axis);

        //Update state
        void CopyCodesEndFrame();

        //Getters
        float GetXStickHor() { return controllerAxis; }

        //Controller buttons
        std::unordered_set<Uint8> currentButton;
        std::unordered_set<Uint8> previousButton;

    private:

        //Controller axis
        float controllerAxis = 0.0f;
        float triggerPressure = 0.0f;

        int instanceId = -1;
    };
}

