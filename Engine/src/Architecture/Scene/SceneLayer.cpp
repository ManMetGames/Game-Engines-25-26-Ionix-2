#include "Architecture/scene/SceneLayer.h"

namespace IonixEngine
{
    void SceneLayer::OnAttach()
    {
        // Nothing for now (I'm not sure how to exactly do it)
    }

    void SceneLayer::OnDetach()
    {
        // Ensure the current scene exits when the layer is detached
        if (m_Current)
        {
            m_Current->OnExit();
            m_Current.reset();
        }
    }

    void SceneLayer::OnUpdate()
    {
        if (m_Current)
            m_Current->OnUpdate();
    }

    void SceneLayer::OnEvent(IonixEvent& e)
    {
        if (m_Current)
            m_Current->OnEvent(e);
    }

    void SceneLayer::SetScene(std::unique_ptr<Scene> scene)
    {
        // Call exit on the previous scene before replacing
        if (m_Current)
            m_Current->OnExit();

        m_Current = std::move(scene);

        // Call enter on the new scene after becoming active
        if (m_Current)
            m_Current->OnEnter();
    }
}
