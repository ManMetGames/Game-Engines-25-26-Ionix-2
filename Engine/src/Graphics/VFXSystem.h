#pragma once

#include <vector>
#include <cstdint>
#include <cstdlib>
#include "SDL.h"
#include "Architecture/ECS/Component.hpp"

namespace IonixEngine {

    // Lightning bolt effect configuration
    struct LightningEffect {
        bool active = false;
        
        // Start and end points
        float x1 = 0.0f;
        float y1 = 0.0f;
        float x2 = 0.0f;
        float y2 = 0.0f;
        
        // Visual properties
        float thickness = 2.0f;
        float jaggedness = 0.1f;    // Percentage of distance for max perpendicular offset
        int segments = 10;           // Number of line segments
        
        // Color
        Uint8 r = 100;
        Uint8 g = 180;
        Uint8 b = 255;
        Uint8 a = 255;
        
        // Lifetime (auto-destroy)
        float lifetime = 0.5f;       // Total lifetime in seconds
        float timeAlive = 0.0f;      // Time elapsed since creation
        bool fadeOut = true;         // Whether to fade alpha over lifetime
        
        // Flicker animation (regenerate path periodically)
        bool flickerEnabled = true;
        float flickerSpeed = 0.05f;  // Time between path regenerations
        float flickerTimer = 0.0f;
        
        // Cached path points (regenerated on flicker)
        std::vector<float> pathX;
        std::vector<float> pathY;
        
        // Render settings
        int renderLayer = 0;
        int zOrder = 0;
    };

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
        
        // Distortion effect (heat haze / gravitational lensing effect)
        bool distortionEnabled = false;     // Enable waviness/warping effect
        float distortionAmplitude = 5.0f;   // How far vertices deviate (in pixels)
        float distortionFrequency = 3.0f;   // Number of waves around the ring
        float distortionSpeed = 2.0f;       // Animation speed
        float distortionTime = 0.0f;        // Time accumulator for animation
        
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
        void SetRingDistortion(int id, bool enabled, float amplitude = 5.0f, float frequency = 3.0f, float speed = 2.0f);
        
        // Getters
        bool IsRingActive(int id) const;
        float GetRingRadius(int id) const;
        bool IsRingDistortionEnabled(int id) const;

        // Lightning effect management
        int CreateLightning(float startX, float startY, float endX, float endY, float lifetime);
        void DestroyLightning(int id);
        
        // Lightning configuration
        void SetLightningPosition(int id, float startX, float startY, float endX, float endY);
        void SetLightningColor(int id, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void SetLightningProperties(int id, float thickness, float jaggedness, int segments);
        void SetLightningFlicker(int id, bool enabled, float speed);
        void SetLightningRenderLayer(int id, int layer, int zOrder = 0);
        void SetLightningFadeOut(int id, bool enabled);
        
        // Lightning getters
        bool IsLightningActive(int id) const;

    private:
        std::vector<RingEffect> m_Rings;
        std::size_t m_MaxRings;
        
        std::vector<LightningEffect> m_Lightnings;
        std::size_t m_MaxLightnings;

        std::size_t GetFreeRingIndex();
        void DrawRing(SDL_Renderer* renderer, const RingEffect& ring);
        void DrawThickCircle(SDL_Renderer* renderer, float centerX, float centerY, 
                            float radius, float thickness, int segments,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        void DrawThickCircleWithDistortion(SDL_Renderer* renderer, float centerX, float centerY,
                            float radius, float thickness, int segments,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                            float distortAmplitude, float distortFrequency, float distortTime);
        
        // Lightning helpers
        std::size_t GetFreeLightningIndex();
        void GenerateLightningPath(LightningEffect& lightning);
        void DrawLightning(SDL_Renderer* renderer, const LightningEffect& lightning);
        void DrawThickLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2,
                          float thickness, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    };
}
