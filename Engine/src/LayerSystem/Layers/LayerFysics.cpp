#include "LayerSystem/Layers/LayerUI.h"
#include "EventSystem/Event.h"

namespace IonixEngine
{
    void LayerFysics::OnAttach(){}
    void LayerFysics::OnDetach() {}
    void LayerFysics::OnUpdate(){} 
    void LayerFysics::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {
            // Add more cases as needed.... (Note: Most engine features don't require events, they
            //                              can just be callable functions.
        }
    }
}