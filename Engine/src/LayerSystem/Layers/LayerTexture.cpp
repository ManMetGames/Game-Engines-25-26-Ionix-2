#include "LayerTexture.hpp"
#include "SDL_image.h"
#include "Architecture/TextureManager/TextureManager.h"

namespace IonixEngine
{
    void LayerTexture::OnAttach() 
    {
        IMG_Init(IMG_INIT_PNG);
        TextureManager::Get().Init();
        TextureManager::Get().AddTexture("./Assets/1.png", "1");
        TextureManager::Get().AddTexture("./Assets/2.png", "2");
        TextureManager::Get().AddTexture("./Assets/3.png", "3");
    }

    void LayerTexture::OnDetach()
    {
        TextureManager::Get().Shutdown();
        IMG_Quit();
    }

    void LayerTexture::OnUpdate() 
    {
    }

    void LayerTexture::OnEvent(IonixEvent& e)
    {

    }
}
