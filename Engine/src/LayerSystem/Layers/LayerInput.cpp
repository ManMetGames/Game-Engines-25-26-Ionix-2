#include "LayerSystem/Layers/LayerInput.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include <iostream>

namespace IonixEngine
{
    void LayerInput::OnAttach() 
    {
        m_Input = new Input();
        m_ControllerManager = new ControllerManager();
    }

    void LayerInput::OnDetach() {}

    void LayerInput::OnUpdate() {}

    void LayerInput::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {                         

        }
    }
}