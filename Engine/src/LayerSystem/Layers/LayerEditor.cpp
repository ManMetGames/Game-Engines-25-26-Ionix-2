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

            case IonixEventType::WindowMaximized:
            {
                auto& windowEvent = static_cast<WindowMaximizedEvent&>(e);
                OnWindowMaximizedEvent(windowEvent);
                break;
            }

            case IonixEventType::WindowResized:
            {
                auto& windowEvent = static_cast<WindowResizedEvent&>(e);
                OnWindowResizedEvent(windowEvent);
                break;
            }

            case IonixEventType::WindowMoved:
            {
                auto& windowEvent = static_cast<WindowMovedEvent&>(e);
                OnWindowMovedEvent(windowEvent);
                break;
            }

            case IonixEventType::WindowHidden:
            {
                auto& windowEvent = static_cast<WindowHiddenEvent&>(e);
                OnWindowHiddenEvent(windowEvent);
                break;
            }

            case IonixEventType::WindowRestored:
            {
                auto& windowEvent = static_cast<WindowRestoredEvent&>(e);
                OnWindowRestoredEvent(windowEvent);
                break;
            }

            // Add more cases as needed.... (Note: Most engine features don't require events, they
            //                              can just be callable functions.
        }
    }

    void LayerEditor::OnWindowClosedEvent(WindowClosedEvent& e)
    {
        std::cout << "IonixEvent: Window closed\n";

        Application::Get().m_Running = false;
        e.Handled = true;
    }
    void LayerEditor::OnWindowMinimizedEvent(WindowMinimizedEvent& e)
    {
        std::cout << "IonixEvent: Window Minimized\n";
        e.Handled = true;
    }
    void LayerEditor::OnWindowMaximizedEvent(WindowMaximizedEvent& e)
    {
        std::cout << "IonixEvent: Window Maximized\n";
        e.Handled = true;
    }

    void LayerEditor::OnWindowResizedEvent(WindowResizedEvent& e)
    {
        std::cout << "IonixEvent: Window Resized to " << e.Width <<"x" << e.Height << "\n";
        e.Handled = true;
    }

    void LayerEditor::OnWindowMovedEvent(WindowMovedEvent& e)
    {
        std::cout << "IonixEvent: Window moved to " << "X:" << e.WinX << " Y:" << e.WinY << "\n";
        e.Handled = true;
    }

 
    void LayerEditor::OnWindowHiddenEvent(WindowHiddenEvent& e)
    {
        std::cout << "IonixEvent: Window has been hidden \n"; 
        e.Handled = true; 
    }

    //Window Restored
    void LayerEditor::OnWindowRestoredEvent(WindowRestoredEvent& e)
    {
        std::cout << "IonixEvent: Window has been Restored to previous size \n";
        e.Handled = true;
    }

    
}