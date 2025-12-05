#pragma once
#include "LayerSystem/Layer.h"
#include "UI/UI.h"

#include "UI/UIManager.h"

#include "UI/FontLoader.h"


namespace IonixEngine
{
    class LayerUI : public Layer
    {
    private:
        UIManager uiManager;
        UI ui;
        Fontloader fontLoader;
    public:
        LayerUI() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;
        UIManager GetUIManager() { return uiManager; }
		UI GetUI() { return ui; }
        UI* m_UI;

        UIManager* m_UIManager;

        Fontloader* m_FontLoader;

    };
}
