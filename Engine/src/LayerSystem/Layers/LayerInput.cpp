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
        if (IsControllerButtonDown(1, static_cast<Uint8>(1)))
        {
            std::cout << " Controller 0 pressed the button\n";
        }
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
        }
    }

    std::unordered_map<int, ControllerManager*>& LayerInput::GetControllerManager()
    {
        return controllerManagers;
    }

    void LayerInput::OnControllerButtonDown(ControllerButtonDownEvent& e)
    {
        controllerManagers[e.instanceId]->SetButtonPressed(e.button);
    }

    bool LayerInput::IsControllerButtonDown(int instanceId, Uint8 button)
    {
        ControllerManager* controllerManager = controllerManagers[instanceId];

        if (controllerManager != nullptr) {
            bool t = !controllerManager->previousButton.count(button) && controllerManager->currentButton.count(button);
            if(t)
                return t; // debugging test statement - streamline this block once done with test
        }
        return false;
    }
}