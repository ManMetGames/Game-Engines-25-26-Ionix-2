#pragma once
#include "Architecture/Macros.h"
#include "EventSystem/Event.h"

namespace IonixEngine
{
    // Base interface for a game Scene (single active scene model)
    class ENGINE_API Scene
    {
    public:
        virtual ~Scene() = default;

        // Called when the scene becomes the active scene
        virtual void OnEnter() {}
        // Called when the scene stops being the active scene
        virtual void OnExit() {}
        // Called once per frame while the scene is active
        virtual void OnUpdate() {}
        // Receives input/window events while the scene is active
        virtual void OnEvent(IonixEvent& e) {}
    };
}
