#include "LayerSystem/Layers/LayerInput.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include <iostream>
#include "Input/ControllerManager.h"
namespace IonixEngine
{
    void LayerInput::OnAttach()
    {
        m_Input = new Input();
    }

    void LayerInput::OnDetach() {}

    void LayerInput::OnUpdate()
    {

    }

    void LayerInput::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {
        case IonixEventType::ControllerButtonDown:
        {
            auto& controllerButtonDown = static_cast<ControllerButtonDownEvent&>(e);
            OnControllerButtonDown(controllerButtonDown);
            break;
        }
        case IonixEventType::ControllerButtonUp:
        {
            auto& controllerButtonUp = static_cast<ControllerButtonUpEvent&>(e);
            OnControllerButtonUp(controllerButtonUp);
            break;
        }
        case IonixEventType::ControllerAxis:
        {
            auto& controllerAxis = static_cast<ControllerAxisEvent&>(e);
            OnControllerAxis(controllerAxis);
            break;
        }
        case IonixEventType::ControllerTrigger:
        {
            auto& controllerTrigger = static_cast<ControllerTriggerEvent&>(e);
            OnControllerTrigger(controllerTrigger);
            break;
        }
        default:
            break;
        }

    }
    void LayerInput::OnControllerButtonDown(ControllerButtonDownEvent& e)
    {
        controllerManagers[e.instanceId]->SetButtonPressed(e.button);
    }

    void LayerInput::OnControllerButtonUp(ControllerButtonUpEvent& e)
    {
        controllerManagers[e.instanceId]->SetButtonReleased(e.button);
    }

    void LayerInput::OnControllerAxis(ControllerAxisEvent& e)
    {
        controllerManagers[e.instanceId]->NormaliseStickAxis(e.instanceId, e.axis, e.direction);
    }

    void LayerInput::OnControllerTrigger(ControllerTriggerEvent& e)
    {
        controllerManagers[e.instanceId]->NormaliseTrigger(e.instanceId, e.pressure, e.trigger);
    }


    bool LayerInput::IsControllerButtonDown(int instanceId, Uint8 button)
    {
        ControllerManager* controllerManager = controllerManagers[instanceId];

        if (controllerManager != nullptr)
            return controllerManager->IsButtonDown(instanceId, button);

        return false;
    }

    bool LayerInput::IsControllerButtonUp(int instanceId, Uint8 button)
    {
        ControllerManager* controllerManager = controllerManagers[instanceId];

        if (controllerManager != nullptr)
            return controllerManager->IsButtonUp(instanceId, button);

        return false;
    }

    bool LayerInput::IsControllerButtonHeld(int instanceId, Uint8 button)
    {
        ControllerManager* controllerManager = controllerManagers[instanceId];

        if (controllerManager != nullptr)
            return controllerManager->IsButtonHeld(instanceId, button);

        return 0.0f;
    }

    float LayerInput::GetControllerAxis(int instanceId, SDL_GameControllerAxis axis)
    {
        ControllerManager* controllerManager = controllerManagers[instanceId];
        
        if (controllerManager != nullptr)
            return controllerManager->GetStickAxis(axis);

        return 0.0f;
    }

    float LayerInput::GetControllerPressure(int instanceId, SDL_GameControllerAxis trigger)
    {
        ControllerManager* controllerManager = controllerManagers[instanceId];

        if (controllerManager != nullptr)
            return controllerManager->GetTriggerPressure(trigger);

        return 0.0f;
    }

}
