#include "SceneLayer.h"
#include "Architecture/Application.h"

namespace IonixEngine
{
    void LayerScene::OnAttach()
    {
        // Nothing for now (I'm not sure how to exactly do it)
        SetScene(std::make_unique<Scene>());
    }

    void LayerScene::OnDetach()
    {
        // Ensure the current scene exits when the layer is detached
        if (m_Current)
        {
            m_Current->OnExit();
            m_Current.reset();
        }
        Handle().scene = nullptr;
    }

    void LayerScene::OnUpdate()
    {
        if (m_Current)
            m_Current->OnUpdate(Application::Get().deltaTime);
    }

    void LayerScene::OnEvent(IonixEvent& e)
    {
        if (m_Current)
            m_Current->OnEvent(e);
    }

    void LayerScene::SetScene(std::unique_ptr<Scene> scene)
    {
        // Call exit on the previous scene before replacing
        if (m_Current)
            m_Current->OnExit();

        m_Current = std::move(scene);
        Handle().scene = m_Current.get();

        // Call enter on the new scene after becoming active
        if (m_Current)
            m_Current->OnEnter();
    }


    Scene* LayerScene::CurrentScene() {
        return LayerScene::Handle().scene;
    }
}