#include "LayerSystem/Layers/LayerInput.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include <iostream>

namespace IonixEngine
{
    void LayerInput::OnAttach() 
    {
        m_Input = new Input();
    }

    void LayerInput::OnDetach() {}

    void LayerInput::OnUpdate() 
    {
        //std::cout << "Input Update Ran.\n";
    }

    void LayerInput::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {
                         
        }
    }

}