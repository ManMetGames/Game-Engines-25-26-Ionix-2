#pragma once
#include <unordered_set>
#include <unordered_map>
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
        void NormaliseStickAxis(int instanceId, float axis, Uint8 direction);
        void NormaliseTrigger(int instanceId, float pressure, Uint8 trigger);

        //Update state
        void CopyCodesEndFrame();

        //Getters
        float GetStickAxis(Uint8 direction) { return controllerAxises[direction]; }
        float GetTriggerPressure(Uint8 trigger) { return controllerTriggers[trigger]; }

    private:

        //Controller buttons
        std::unordered_set<Uint8> currentButton;
        std::unordered_set<Uint8> previousButton;
        std::unordered_map<Uint8, float> controllerAxises;
        std::unordered_map<Uint8, float> controllerTriggers;

        //Controller axis
        float controllerAxis = 0.0f;
        float triggerPressure = 0.0f;
        Uint8 axisDirection;
        float deadZone = 0.04f;

        int instanceId = -1;
    };
}

