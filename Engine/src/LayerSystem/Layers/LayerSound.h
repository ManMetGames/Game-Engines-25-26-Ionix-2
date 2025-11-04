#pragma once
#include "LayerSystem/Layer.h"
#include "Architecture/AudioSystem/SoundManager.h"  
// #include "Scripting/Audio/AudioScripting.h"
#include <SDL.h>          

namespace IonixEngine
{
    class LayerSound : public Layer
    {
    public:
        LayerSound();
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;

    private:
    };
}
