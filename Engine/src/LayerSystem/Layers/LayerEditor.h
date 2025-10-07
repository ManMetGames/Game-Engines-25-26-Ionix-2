#pragma once
#include "LayerSystem/Layer.h"

namespace IonixEngine
{
    class LayerEditor : public Layer
    {
    public:
        LayerEditor() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;

    private:
        void OnWindowClosedEvent(WindowClosedEvent& e);
        void OnWindowMinimizedEvent(WindowMinimizedEvent& e);
        void OnWindowMaximizedEvent(WindowMaximizedEvent& e);
        void OnWindowResizedEvent(WindowResizedEvent& e);
        void OnWindowMovedEvent(WindowMovedEvent& e);
        void OnWindowHiddenEvent(WindowHiddenEvent& e);
    };
}