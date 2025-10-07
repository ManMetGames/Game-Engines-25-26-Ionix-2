#include "LayerSystem/Layers/LayerGraphics.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include <iostream>

namespace IonixEngine
{
    LayerGraphics::LayerGraphics() {
     

    }

    void LayerGraphics::OnAttach() {}

    void LayerGraphics::OnDetach() {}

    void LayerGraphics::OnUpdate() {

        img = IMG_LoadTexture(Application::Get().GetWindow().GetSdlRenderer(), "aur naur.jpg");
        SDL_Rect texr;
        texr.x = 0;
        texr.y = 0;
        texr.w = 800;
        texr.h = 600;



        SDL_SetRenderDrawColor(Application::Get().GetWindow().GetSdlRenderer(), 255, 255, 255, 0);


        SDL_RenderClear(Application::Get().GetWindow().GetSdlRenderer());
        SDL_RenderCopy(Application::Get().GetWindow().GetSdlRenderer(), img, nullptr, &texr);
        SDL_RenderPresent(Application::Get().GetWindow().GetSdlRenderer());
    }

    void LayerGraphics::OnEvent(IonixEvent& e) {}
 }

