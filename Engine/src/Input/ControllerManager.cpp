#include "ControllerManager.h"
#include <SDL_stdinc.h>


namespace IonixEngine
{
    //Controller buttons
    bool ControllerManager::IsButtonDown(int instanceId, Uint8 btn) const
    {
        if (instanceId != this->instanceId) return false;

        return !previousButton.count(btn) && currentButton.count(btn);
    }

    bool ControllerManager::IsButtonUp(int instanceId, Uint8 btn) const
    {
        if (instanceId != this->instanceId) return false;

        return previousButton.count(btn) && !currentButton.count(btn);
    }

    bool ControllerManager::IsButtonHeld(int instanceId, Uint8 btn) const
    {
        if (instanceId != this->instanceId) return false;

        return currentButton.count(btn);
    }

    void ControllerManager::SetButtonPressed(Uint8 btn)
    {
        currentButton.insert(btn);
    }

    void ControllerManager::SetButtonReleased(Uint8 btn)
    {   
        currentButton.erase(btn);
    }

    //Controller axis
    void ControllerManager::NormaliseStickAxis(int instanceId, float axis, Uint8 direction)
    {
        if (instanceId == this->instanceId)
            controllerAxis = static_cast<float>(axis) / 32768.0f;
        if (fabsf(controllerAxis) <= deadZone) {
            controllerAxis = 0.0f;
            }
            controllerAxises[direction] = controllerAxis;
        
    }
    
    void ControllerManager::NormaliseTrigger(int instanceId, float pressure, Uint8 trigger)
    {
        if (instanceId == this->instanceId)
            triggerPressure = static_cast<float>(pressure) / 32767.0f;
            triggerPressure = std::round(triggerPressure * 100.0f) / 100.0f;
            controllerTriggers[trigger] = triggerPressure;
        
    }

    //Frame management
    void ControllerManager::CopyCodesEndFrame()
    {
            previousButton = currentButton;
    }
}
