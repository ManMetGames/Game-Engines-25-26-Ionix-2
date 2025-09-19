#include "LayerSystem/Layers/LayerEditor.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include <iostream>

namespace IonixEngine
{
    void LayerEditor::OnAttach() {}

    void LayerEditor::OnDetach() {}

    void LayerEditor::OnUpdate() {}

    void LayerEditor::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {
            case IonixEventType::WindowClosed:
            {
                auto& windowEvent = static_cast<WindowClosedEvent&>(e);
                OnWindowClosedEvent(windowEvent);
                break;
            }

            case IonixEventType::WindowMinimized:
            {
                auto& windowEvent = static_cast<WindowMinimizedEvent&>(e);
                OnWindowMinimizedEvent(windowEvent);
                break;
            }

            // Add more cases as needed.... (Note: Most engine features don't require events, they
            //                              can just be callable functions.
        }
    }

    void LayerEditor::OnWindowClosedEvent(WindowClosedEvent& e)
    {
        std::cout << "IonixEvent: Window closed";

        Application::Get().m_Running = false;
        e.Handled = true;
    }

    void LayerEditor::OnWindowMinimizedEvent(WindowMinimizedEvent& e)
    {
        std::cout << "IonixEvent: Winow minimized";
        e.Handled = true;
    }
}