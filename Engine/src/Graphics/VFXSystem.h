#pragma once

#include <vector>
#include <cstdint>
#include "SDL.h"
#include "Architecture/ECS/Component.hpp"

namespace IonixEngine {

    // Ring effect configuration
    struct RingEffect {
        bool active = false;
        
        // Position (can follow an entity or be static)
        float x = 0.0f;
        float y = 0.0f;
        
        // Ring properties
        float radius = 100.0f;        // Outer radius of the ring
        float thickness = 2.0f;       // Line thickness (how "wide" the ring stroke is)
        int segments = 64;            // Number of line segments for circle approximation
        
        // Color
        Uint8 r = 255;
        Uint8 g = 255;
        Uint8 b = 255;
        Uint8 a = 255;
        
        // Animation (optional pulsing effect)
        bool pulsing = false;
        float pulseSpeed = 2.0f;      // Pulses per second
        float pulseMinAlpha = 100;    // Minimum alpha during pulse
        float pulseMaxAlpha = 255;    // Maximum alpha during pulse
        float pulseTimer = 0.0f;
        
        // Entity following (if entityId is valid, ring follows that entity)
        int entityId = -1;            // -1 means static position
        float offsetX = 0.0f;         // Offset from entity center
        float offsetY = 0.0f;
        
        // Render settings
        int renderLayer = 0;
        int zOrder = 0;
    };

    class VFXSystem {
    public:
        VFXSystem();

        void Init();
        void Shutdown();
        void Update(float dt);
        void Render(SDL_Renderer* renderer, int currentRenderLayer);
        void Clear();

        // Ring effect management
        int CreateRing(float x, float y, float radius, float thickness = 2.0f);
        void DestroyRing(int id);
        
        // Ring configuration
        void SetRingPosition(int id, float x, float y);
        void SetRingRadius(int id, float radius);
        void SetRingThickness(int id, float thickness);
        void SetRingColor(int id, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void SetRingPulsing(int id, bool enabled, float speed = 2.0f, float minAlpha = 100, float maxAlpha = 255);
        void SetRingFollowEntity(int id, int entityId, float offsetX = 0.0f, float offsetY = 0.0f);
        void SetRingRenderLayer(int id, int layer, int zOrder = 0);
        void SetRingSegments(int id, int segments);
        
        // Getters
        bool IsRingActive(int id) const;
        float GetRingRadius(int id) const;

    private:
        std::vector<RingEffect> m_Rings;
        std::size_t m_MaxRings;

        std::size_t GetFreeRingIndex();
        void DrawRing(SDL_Renderer* renderer, const RingEffect& ring);
        void DrawThickCircle(SDL_Renderer* renderer, float centerX, float centerY, 
                            float radius, float thickness, int segments,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    };
}
