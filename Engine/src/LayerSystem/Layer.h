#pragma once
#include "EventSystem/Event.h"
#include <string>

namespace IonixEngine
{
    class Layer
    {
    public:
        Layer() { }
        virtual ~Layer() = default;

        virtual void OnAttach() { }
        virtual void OnDetach() { }
        virtual void OnUpdate() { }
        virtual void OnEvent(IonixEvent& e) { }
    };
}

