#pragma once
#include "EventSystem/Event.h"
#include <string>

namespace IonixEngine
{
    class Layer
    {
    public:
        Layer(int id = -1) : m_LayerID(id) { }
        virtual ~Layer() = default;

        virtual void OnAttach() { }
        virtual void OnDetach() { }
        virtual void OnUpdate() { }
        virtual void OnEvent(IonixEvent& e) { }

        int GetID() const { return m_LayerID; }

    private:
        int m_LayerID;
    };
}

