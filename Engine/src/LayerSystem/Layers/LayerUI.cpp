#include "LayerSystem/Layers/LayerUI.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include <iostream>

namespace IonixEngine
{
    void LayerUI::OnAttach() {}

    void LayerUI::OnDetach() {}

    void LayerUI::OnUpdate() {}

    void LayerUI::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {
            // Add more cases as needed.... (Note: Most engine features don't require events, they
            //                              can just be callable functions.
        }
    }
}