#include "LayerSystem/Layers/LayerTexture.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include "Architecture/TextureManager/TextureManager.h"
#include "Architecture/TextureManager/TextureData.h"
#include <stdio.h>
#include <SDL.h>
#include <iostream>

namespace IonixEngine
{
    void LayerTexture::OnAttach() 
    {
        IMG_Init(IMG_INIT_PNG);
        TextureManager().Get();
    }

    void LayerTexture::OnDetach()
    {
        TextureManager().Get().Shutdown();
    }

    void LayerTexture::OnUpdate() 
    {
    }

    void LayerTexture::OnEvent(IonixEvent& e)
    {

    }
}