#pragma once
#include "Architecture/Macros.h"
#include "LayerSystem/Layer.h"
#include "Scene.h"
#include <memory>

namespace IonixEngine
{
    // Layer that hosts the single active Scene and bridges updates/events to it
    class ENGINE_API LayerScene : public Layer
    {
    public:
        LayerScene() = default;
        virtual ~LayerScene() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate() override;
        void OnEvent(IonixEvent& e) override;

        // Replace the active scene. Calls OnExit on the old scene (if any),
        // then OnEnter on the new scene.
        void SetScene(std::unique_ptr<Scene> scene);

        // Access the current scene (non-owning)
        Scene* GetScene() const { return m_Current.get(); }

    private:
        std::unique_ptr<Scene> m_Current;
    };
}
