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

    bool LayerInput::IsControllerButtonDown(int instanceId, Uint8 button)
    {
        ControllerManager* controllerManager = controllerManagers[instanceId];

        if(controllerManager != nullptr)
            return controllerManager->IsButtonDown(instanceId, button);
    }

    bool LayerInput::IsControllerButtonUp(int instanceId, Uint8 button)
    {
        ControllerManager* controllerManager = controllerManagers[instanceId];

        if (controllerManager != nullptr)
            return controllerManager->IsButtonUp(instanceId, button);
    }
}