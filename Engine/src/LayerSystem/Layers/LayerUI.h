#pragma once
#include "LayerSystem/Layer.h"
#include "UI/UI.h"
#include "UI/UIManager.h"

namespace IonixEngine
{
    class LayerUI : public Layer
    {
    private:
        UIManager uiManager;
    public:
        LayerUI() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;
        UIManager GetUIManager() { return uiManager; }
        UI* m_UI;
        UIManager* m_UIManager;
    };
}