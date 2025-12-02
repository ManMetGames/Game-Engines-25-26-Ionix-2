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
            controllerAxis = std::round(controllerAxis * 100.0f) / 100.0f;
            controllerAxises[direction] = controllerAxis;
        
    }
    
    void ControllerManager::NormaliseTrigger(int instanceId, float axis)
    {
        if (instanceId == this->instanceId)
            triggerPressure = static_cast<float>(axis) / 32767.0f;
            triggerPressure = std::round(triggerPressure * 100.0f) / 100.0f;
        
    }

    //Frame management
    void ControllerManager::CopyCodesEndFrame()
    {
            previousButton = currentButton;
    }
}
