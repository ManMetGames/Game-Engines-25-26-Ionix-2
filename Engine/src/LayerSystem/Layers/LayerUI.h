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
        //UIManager uiManager;
        //UI ui;
    public:
        LayerUI()
        {

        }

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;
        UIManager* m_UIManager;
        UI* m_UI;
        UIManager* GetUIManager() { return m_UIManager; }
        UI* GetUI() { return m_UI; }
        Fontloader* m_FontLoader;
    };
}
