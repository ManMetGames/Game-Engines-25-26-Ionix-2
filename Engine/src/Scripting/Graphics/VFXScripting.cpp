#include "Scripting/Graphics/VFXScripting.h"
#include "Architecture/Application.h"
#include "Architecture/Scene.h"
#include "Graphics/VFXSystem.h"

namespace IonixEngine {

    VFXScripting* VFXScripting::s_Instance = nullptr;

    VFXScripting& VFXScripting::Get() {
        if (!s_Instance)
            s_Instance = new VFXScripting();
        return *s_Instance;
    }

    void VFXScripting::Init(sol::state& lua) {
        
        // Create a ring effect
        // Returns: ring ID (int), or -1 if failed
        auto createRing = [](float x, float y, float radius, sol::optional<float> thickness) -> int {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return -1;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            return scene->GetVFXSystem().CreateRing(x, y, radius, thickness.value_or(2.0f));
        };

        // Destroy a ring effect
        auto destroyRing = [](int id) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().DestroyRing(id);
        };

        // Set ring position
        auto setRingPosition = [](int id, float x, float y) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetRingPosition(id, x, y);
        };

        // Set ring radius
        auto setRingRadius = [](int id, float radius) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetRingRadius(id, radius);
        };

        // Set ring thickness
        auto setRingThickness = [](int id, float thickness) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetRingThickness(id, thickness);
        };

        // Set ring color (RGBA)
        auto setRingColor = [](int id, int r, int g, int b, sol::optional<int> a) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetRingColor(
                id,
                static_cast<Uint8>(r),
                static_cast<Uint8>(g),
                static_cast<Uint8>(b),
                static_cast<Uint8>(a.value_or(255))
            );
        };

        // Set ring pulsing animation
        auto setRingPulsing = [](int id, bool enabled, sol::optional<float> speed, 
                                 sol::optional<float> minAlpha, sol::optional<float> maxAlpha) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetRingPulsing(
                id, 
                enabled, 
                speed.value_or(2.0f),
                minAlpha.value_or(100.0f),
                maxAlpha.value_or(255.0f)
            );
        };

        // Set ring to follow an entity
        auto setRingFollowEntity = [](int id, int entityId, sol::optional<float> offsetX, sol::optional<float> offsetY) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetRingFollowEntity(
                id, 
                entityId, 
                offsetX.value_or(0.0f), 
                offsetY.value_or(0.0f)
            );
        };

        // Set ring render layer and z-order
        auto setRingRenderLayer = [](int id, int layer, sol::optional<int> zOrder) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetRingRenderLayer(id, layer, zOrder.value_or(0));
        };

        // Set ring segment count (quality)
        auto setRingSegments = [](int id, int segments) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetRingSegments(id, segments);
        };

        // Set ring distortion effect (heat haze / gravitational lensing)
        auto setRingDistortion = [](int id, bool enabled, sol::optional<float> amplitude,
                                    sol::optional<float> frequency, sol::optional<float> speed) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetRingDistortion(
                id,
                enabled,
                amplitude.value_or(5.0f),
                frequency.value_or(3.0f),
                speed.value_or(2.0f)
            );
        };

        // Check if ring is active
        auto isRingActive = [](int id) -> bool {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return false;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            return scene->GetVFXSystem().IsRingActive(id);
        };

        // Get ring radius
        auto getRingRadius = [](int id) -> float {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return 0.0f;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            return scene->GetVFXSystem().GetRingRadius(id);
        };

        // Check if ring distortion is enabled
        auto isRingDistortionEnabled = [](int id) -> bool {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return false;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            return scene->GetVFXSystem().IsRingDistortionEnabled(id);
        };

        // Clear all VFX effects
        auto clearVFX = []() {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().Clear();
        };

        // =========================================================================
        // LIGHTNING VFX BINDINGS
        // =========================================================================

        // Create a lightning bolt effect between two points
        // Returns: lightning ID (int), or -1 if failed
        auto createLightning = [](float startX, float startY, float endX, float endY, 
                                  sol::optional<float> lifetime) -> int {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return -1;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            return scene->GetVFXSystem().CreateLightning(startX, startY, endX, endY, 
                                                         lifetime.value_or(0.5f));
        };

        // Destroy a lightning effect
        auto destroyLightning = [](int id) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().DestroyLightning(id);
        };

        // Set lightning color (RGBA)
        auto setLightningColor = [](int id, int r, int g, int b, sol::optional<int> a) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetLightningColor(
                id,
                static_cast<Uint8>(r),
                static_cast<Uint8>(g),
                static_cast<Uint8>(b),
                static_cast<Uint8>(a.value_or(255))
            );
        };

        // Set lightning visual properties (thickness, jaggedness, segments)
        auto setLightningProperties = [](int id, sol::optional<float> thickness, 
                                        sol::optional<float> jaggedness, 
                                        sol::optional<int> segments) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            
            // Get current properties if not specified
            float th = thickness.value_or(2.0f);
            float jag = jaggedness.value_or(0.1f);
            int seg = segments.value_or(10);
            
            scene->GetVFXSystem().SetLightningProperties(id, th, jag, seg);
        };

        // Set lightning flicker animation
        auto setLightningFlicker = [](int id, bool enabled, sol::optional<float> speed) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetLightningFlicker(id, enabled, speed.value_or(0.05f));
        };

        // Set lightning render layer and z-order
        auto setLightningRenderLayer = [](int id, int layer, sol::optional<int> zOrder) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetLightningRenderLayer(id, layer, zOrder.value_or(0));
        };

        // Set lightning fade out
        auto setLightningFadeOut = [](int id, bool enabled) {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            scene->GetVFXSystem().SetLightningFadeOut(id, enabled);
        };

        // Check if lightning is active
        auto isLightningActive = [](int id) -> bool {
            if (!Application::Get().layerScene || !Application::Get().layerScene->GetScene()) {
                return false;
            }
            Scene* scene = Application::Get().layerScene->GetScene();
            return scene->GetVFXSystem().IsLightningActive(id);
        };

        // Register VFX table with Lua
        lua["VFX"] = lua.create_table_with(
            // Ring creation and destruction
            "create_ring", createRing,
            "destroy_ring", destroyRing,
            
            // Ring configuration
            "set_ring_position", setRingPosition,
            "set_ring_radius", setRingRadius,
            "set_ring_thickness", setRingThickness,
            "set_ring_color", setRingColor,
            "set_ring_pulsing", setRingPulsing,
            "set_ring_follow_entity", setRingFollowEntity,
            "set_ring_render_layer", setRingRenderLayer,
            "set_ring_segments", setRingSegments,
            "set_ring_distortion", setRingDistortion,
            
            // Ring queries
            "is_ring_active", isRingActive,
            "get_ring_radius", getRingRadius,
            "is_ring_distortion_enabled", isRingDistortionEnabled,
            
            // Lightning creation and destruction
            "create_lightning", createLightning,
            "destroy_lightning", destroyLightning,
            
            // Lightning configuration
            "set_lightning_color", setLightningColor,
            "set_lightning_properties", setLightningProperties,
            "set_lightning_flicker", setLightningFlicker,
            "set_lightning_render_layer", setLightningRenderLayer,
            "set_lightning_fade_out", setLightningFadeOut,
            
            // Lightning queries
            "is_lightning_active", isLightningActive,
            
            // General VFX control
            "clear", clearVFX
        );
    }
}
