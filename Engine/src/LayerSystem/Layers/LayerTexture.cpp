#include "LayerTexture.hpp"
#include "SDL_image.h"
#include "Architecture/TextureManager/TextureManager.h"

namespace IonixEngine
{
    void LayerTexture::OnAttach() 
    {
        IMG_Init(IMG_INIT_PNG);
        TextureManager().Get().Init();
    }

    void LayerTexture::OnDetach()
    {
        TextureManager().Get().Shutdown();
        IMG_Quit();
    }

    void LayerTexture::OnUpdate() 
    {
    }

    void LayerTexture::OnEvent(IonixEvent& e)
    {

    }
}
