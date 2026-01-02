#include "Graphics/VFXSystem.h"
#include "Graphics/SpriteComponent.h"
#include "Architecture/Application.h"
#include "Architecture/Scene.h"
#include "Architecture/ECS/Temp_Vec2.hpp"
#include <cmath>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace IonixEngine {

    VFXSystem::VFXSystem()
        : m_MaxRings(64), m_MaxLightnings(64) {
    }

    void VFXSystem::Init() {
        m_Rings.clear();
        m_Rings.reserve(m_MaxRings);
        m_Lightnings.clear();
        m_Lightnings.reserve(m_MaxLightnings);
    }

    void VFXSystem::Shutdown() {
        Clear();
    }

    void VFXSystem::Clear() {
        m_Rings.clear();
        m_Lightnings.clear();
    }

    std::size_t VFXSystem::GetFreeRingIndex() {
        for (std::size_t i = 0; i < m_Rings.size(); ++i) {
            if (!m_Rings[i].active) {
                return i;
            }
        }

        if (m_Rings.size() < m_MaxRings) {
            m_Rings.push_back(RingEffect{});
            return m_Rings.size() - 1;
        }

        return m_MaxRings;
    }

    int VFXSystem::CreateRing(float x, float y, float radius, float thickness) {
        std::size_t index = GetFreeRingIndex();
        if (index >= m_MaxRings) {
            return -1;
        }

        if (index >= m_Rings.size()) {
            m_Rings.push_back(RingEffect{});
        }

        RingEffect& ring = m_Rings[index];
        ring.active = true;
        ring.x = x;
        ring.y = y;
        ring.radius = radius;
        ring.thickness = thickness;
        ring.segments = 64;
        ring.r = 255;
        ring.g = 255;
        ring.b = 255;
        ring.a = 255;
        ring.pulsing = false;
        ring.pulseTimer = 0.0f;
        ring.entityId = -1;
        ring.offsetX = 0.0f;
        ring.offsetY = 0.0f;
        ring.renderLayer = 0;
        ring.zOrder = 0;
        ring.distortionEnabled = false;
        ring.distortionAmplitude = 5.0f;
        ring.distortionFrequency = 3.0f;
        ring.distortionSpeed = 2.0f;
        ring.distortionTime = 0.0f;

        return static_cast<int>(index);
    }

    void VFXSystem::DestroyRing(int id) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size()) {
            return;
        }
        m_Rings[id].active = false;
    }

    void VFXSystem::SetRingPosition(int id, float x, float y) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size() || !m_Rings[id].active) {
            return;
        }
        m_Rings[id].x = x;
        m_Rings[id].y = y;
    }

    void VFXSystem::SetRingRadius(int id, float radius) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size() || !m_Rings[id].active) {
            return;
        }
        m_Rings[id].radius = radius;
    }

    void VFXSystem::SetRingThickness(int id, float thickness) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size() || !m_Rings[id].active) {
            return;
        }
        m_Rings[id].thickness = thickness;
    }

    void VFXSystem::SetRingColor(int id, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size() || !m_Rings[id].active) {
            return;
        }
        m_Rings[id].r = r;
        m_Rings[id].g = g;
        m_Rings[id].b = b;
        m_Rings[id].a = a;
    }

    void VFXSystem::SetRingPulsing(int id, bool enabled, float speed, float minAlpha, float maxAlpha) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size() || !m_Rings[id].active) {
            return;
        }
        m_Rings[id].pulsing = enabled;
        m_Rings[id].pulseSpeed = speed;
        m_Rings[id].pulseMinAlpha = minAlpha;
        m_Rings[id].pulseMaxAlpha = maxAlpha;
    }

    void VFXSystem::SetRingFollowEntity(int id, int entityId, float offsetX, float offsetY) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size() || !m_Rings[id].active) {
            return;
        }
        m_Rings[id].entityId = entityId;
        m_Rings[id].offsetX = offsetX;
        m_Rings[id].offsetY = offsetY;
    }

    void VFXSystem::SetRingRenderLayer(int id, int layer, int zOrder) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size() || !m_Rings[id].active) {
            return;
        }
        m_Rings[id].renderLayer = layer;
        m_Rings[id].zOrder = zOrder;
    }

    void VFXSystem::SetRingSegments(int id, int segments) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size() || !m_Rings[id].active) {
            return;
        }
        m_Rings[id].segments = segments > 8 ? segments : 8; // Minimum 8 segments
    }

    void VFXSystem::SetRingDistortion(int id, bool enabled, float amplitude, float frequency, float speed) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size() || !m_Rings[id].active) {
            return;
        }
        m_Rings[id].distortionEnabled = enabled;
        m_Rings[id].distortionAmplitude = amplitude;
        m_Rings[id].distortionFrequency = frequency;
        m_Rings[id].distortionSpeed = speed;
    }

    bool VFXSystem::IsRingActive(int id) const {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size()) {
            return false;
        }
        return m_Rings[id].active;
    }

    float VFXSystem::GetRingRadius(int id) const {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size()) {
            return 0.0f;
        }
        return m_Rings[id].radius;
    }

    bool VFXSystem::IsRingDistortionEnabled(int id) const {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Rings.size()) {
            return false;
        }
        return m_Rings[id].distortionEnabled;
    }

    void VFXSystem::Update(float dt) {
        for (std::size_t i = 0; i < m_Rings.size(); ++i) {
            RingEffect& ring = m_Rings[i];
            if (!ring.active) {
                continue;
            }

            // Update pulse animation
            if (ring.pulsing) {
                ring.pulseTimer += dt * ring.pulseSpeed;
                if (ring.pulseTimer > 2.0f * static_cast<float>(M_PI)) {
                    ring.pulseTimer -= 2.0f * static_cast<float>(M_PI);
                }
            }

            // Update distortion animation
            if (ring.distortionEnabled) {
                ring.distortionTime += dt * ring.distortionSpeed;
                if (ring.distortionTime > 2.0f * static_cast<float>(M_PI)) {
                    ring.distortionTime -= 2.0f * static_cast<float>(M_PI);
                }
            }

            // Update position from entity if following
            if (ring.entityId >= 0) {
                Scene* scene = Application::Get().layerScene ? Application::Get().layerScene->GetScene() : nullptr;
                if (scene) {
                    Entity* entity = scene->GetEntityFromID(static_cast<EntityID>(ring.entityId));
                    if (entity) {
                        // Get entity position using transform
                        Vec2 entityPos = entity->transform.GetGlobalPosition();
                        float entityX = entityPos.x;
                        float entityY = entityPos.y;
                        
                        // Get entity size if it has a sprite component for center calculation
                        float halfWidth = 0.0f;
                        float halfHeight = 0.0f;
                        SpriteComponent* sprite = entity->GetComponent<SpriteComponent>();
                        if (sprite) {
                            halfWidth = sprite->getSpriteWidth() / 2.0f;
                            halfHeight = sprite->getSpriteHeight() / 2.0f;
                        }
                        
                        ring.x = entityX + halfWidth + ring.offsetX;
                        ring.y = entityY + halfHeight + ring.offsetY;
                    }
                }
            }
        }
        
        // Update lightning effects
        for (std::size_t i = 0; i < m_Lightnings.size(); ++i) {
            LightningEffect& lightning = m_Lightnings[i];
            if (!lightning.active) {
                continue;
            }
            
            // Update lifetime
            lightning.timeAlive += dt;
            if (lightning.timeAlive >= lightning.lifetime) {
                lightning.active = false;
                lightning.pathX.clear();
                lightning.pathY.clear();
                lightning.segmentCorruption.clear();
                continue;
            }
            
            // Update flicker animation
            if (lightning.flickerEnabled) {
                lightning.flickerTimer += dt;
                if (lightning.flickerTimer >= lightning.flickerSpeed) {
                    lightning.flickerTimer = 0.0f;
                    GenerateLightningPath(lightning);
                }
            }
            
            // Update corruption system (decay and spread)
            if (lightning.corruptionEnabled && !lightning.segmentCorruption.empty()) {
                int numSegments = static_cast<int>(lightning.segmentCorruption.size());
                
                // Create a copy to calculate spread without affecting current values
                std::vector<float> spreadDelta(numSegments, 0.0f);
                
                // Calculate spread from each segment to neighbors
                for (int seg = 0; seg < numSegments; ++seg) {
                    float corruption = lightning.segmentCorruption[seg];
                    if (corruption > 0.0f) {
                        // Spread to left neighbor
                        if (seg > 0) {
                            float diff = corruption - lightning.segmentCorruption[seg - 1];
                            if (diff > 0.0f) {
                                spreadDelta[seg - 1] += diff * lightning.corruptionSpreadRate * dt;
                            }
                        }
                        // Spread to right neighbor
                        if (seg < numSegments - 1) {
                            float diff = corruption - lightning.segmentCorruption[seg + 1];
                            if (diff > 0.0f) {
                                spreadDelta[seg + 1] += diff * lightning.corruptionSpreadRate * dt;
                            }
                        }
                    }
                }
                
                // Apply spread and decay
                for (int seg = 0; seg < numSegments; ++seg) {
                    // Apply spread
                    lightning.segmentCorruption[seg] += spreadDelta[seg];
                    
                    // Apply decay
                    lightning.segmentCorruption[seg] -= lightning.corruptionDecayRate * dt;
                    
                    // Clamp to valid range
                    if (lightning.segmentCorruption[seg] < 0.0f) {
                        lightning.segmentCorruption[seg] = 0.0f;
                    } else if (lightning.segmentCorruption[seg] > 1.0f) {
                        lightning.segmentCorruption[seg] = 1.0f;
                    }
                }
            }
        }
    }

    void VFXSystem::Render(SDL_Renderer* renderer, int currentRenderLayer) {
        if (!renderer) {
            return;
        }

        for (std::size_t i = 0; i < m_Rings.size(); ++i) {
            const RingEffect& ring = m_Rings[i];
            if (!ring.active || ring.renderLayer != currentRenderLayer) {
                continue;
            }
            DrawRing(renderer, ring);
        }
        
        // Render lightning effects
        for (std::size_t i = 0; i < m_Lightnings.size(); ++i) {
            const LightningEffect& lightning = m_Lightnings[i];
            if (!lightning.active || lightning.renderLayer != currentRenderLayer) {
                continue;
            }
            DrawLightning(renderer, lightning);
        }
    }

    void VFXSystem::DrawRing(SDL_Renderer* renderer, const RingEffect& ring) {
        Uint8 finalAlpha = ring.a;
        
        // Apply pulsing effect to alpha
        if (ring.pulsing) {
            float pulse = (std::sin(ring.pulseTimer) + 1.0f) * 0.5f; // 0 to 1
            finalAlpha = static_cast<Uint8>(ring.pulseMinAlpha + pulse * (ring.pulseMaxAlpha - ring.pulseMinAlpha));
        }

        // Pass distortion parameters to drawing function
        if (ring.distortionEnabled) {
            DrawThickCircleWithDistortion(renderer, ring.x, ring.y, ring.radius, ring.thickness, 
                                         ring.segments, ring.r, ring.g, ring.b, finalAlpha,
                                         ring.distortionAmplitude, ring.distortionFrequency, ring.distortionTime);
        } else {
            DrawThickCircle(renderer, ring.x, ring.y, ring.radius, ring.thickness, 
                           ring.segments, ring.r, ring.g, ring.b, finalAlpha);
        }
    }

    void VFXSystem::DrawThickCircle(SDL_Renderer* renderer, float centerX, float centerY,
                                    float radius, float thickness, int segments,
                                    Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        // Save current renderer state
        SDL_BlendMode previousBlendMode;
        SDL_GetRenderDrawBlendMode(renderer, &previousBlendMode);
        Uint8 prevR, prevG, prevB, prevA;
        SDL_GetRenderDrawColor(renderer, &prevR, &prevG, &prevB, &prevA);

        // Enable blending for alpha
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);

        float innerRadius = radius - thickness / 2.0f;
        float outerRadius = radius + thickness / 2.0f;
        
        if (innerRadius < 0) innerRadius = 0;

        float angleStep = 2.0f * static_cast<float>(M_PI) / static_cast<float>(segments);

        // Draw multiple concentric circles to create thickness effect
        // For small thickness, we just draw lines between points
        if (thickness <= 2.0f) {
            // Simple single-pixel thick ring using connected lines
            for (int i = 0; i < segments; ++i) {
                float angle1 = static_cast<float>(i) * angleStep;
                float angle2 = static_cast<float>(i + 1) * angleStep;

                int x1 = static_cast<int>(centerX + radius * std::cos(angle1));
                int y1 = static_cast<int>(centerY + radius * std::sin(angle1));
                int x2 = static_cast<int>(centerX + radius * std::cos(angle2));
                int y2 = static_cast<int>(centerY + radius * std::sin(angle2));

                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        } else {
            // For thicker rings, draw filled triangles between inner and outer circles
            // This creates a solid ring effect
            for (int i = 0; i < segments; ++i) {
                float angle1 = static_cast<float>(i) * angleStep;
                float angle2 = static_cast<float>(i + 1) * angleStep;

                float cos1 = std::cos(angle1);
                float sin1 = std::sin(angle1);
                float cos2 = std::cos(angle2);
                float sin2 = std::sin(angle2);

                // Inner and outer points for this segment
                float innerX1 = centerX + innerRadius * cos1;
                float innerY1 = centerY + innerRadius * sin1;
                float outerX1 = centerX + outerRadius * cos1;
                float outerY1 = centerY + outerRadius * sin1;
                float innerX2 = centerX + innerRadius * cos2;
                float innerY2 = centerY + innerRadius * sin2;
                float outerX2 = centerX + outerRadius * cos2;
                float outerY2 = centerY + outerRadius * sin2;

                // Draw the quad as lines (SDL doesn't have filled polygon, so we draw multiple lines)
                // Draw lines from inner to outer for a thick effect
                int numThicknessLines = static_cast<int>(thickness / 1.0f) + 1;
                for (int t = 0; t <= numThicknessLines; ++t) {
                    float tFactor = static_cast<float>(t) / static_cast<float>(numThicknessLines);
                    float currentRadius = innerRadius + tFactor * (outerRadius - innerRadius);
                    
                    int x1 = static_cast<int>(centerX + currentRadius * cos1);
                    int y1 = static_cast<int>(centerY + currentRadius * sin1);
                    int x2 = static_cast<int>(centerX + currentRadius * cos2);
                    int y2 = static_cast<int>(centerY + currentRadius * sin2);
                    
                    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                }
            }
        }

        // Restore previous renderer state
        SDL_SetRenderDrawBlendMode(renderer, previousBlendMode);
        SDL_SetRenderDrawColor(renderer, prevR, prevG, prevB, prevA);
    }

    void VFXSystem::DrawThickCircleWithDistortion(SDL_Renderer* renderer, float centerX, float centerY,
                                                  float radius, float thickness, int segments,
                                                  Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                                  float distortAmplitude, float distortFrequency, float distortTime) {
        // Save current renderer state
        SDL_BlendMode previousBlendMode;
        SDL_GetRenderDrawBlendMode(renderer, &previousBlendMode);
        Uint8 prevR, prevG, prevB, prevA;
        SDL_GetRenderDrawColor(renderer, &prevR, &prevG, &prevB, &prevA);

        // Enable blending for alpha
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);

        float innerRadius = radius - thickness / 2.0f;
        float outerRadius = radius + thickness / 2.0f;
        
        if (innerRadius < 0) innerRadius = 0;

        // Increase segment count for smoother distortion to prevent square-like appearance
        // More segments = smoother curves even with high distortion
        int effectiveSegments = segments * 2; // Double the segments for distortion smoothness
        float angleStep = 2.0f * static_cast<float>(M_PI) / static_cast<float>(effectiveSegments);

        // Draw with distortion effect applied to vertices
        // The distortion creates a wave/warping effect that travels around the ring
        if (thickness <= 2.0f) {
            // Simple single-pixel thick ring with distortion
            for (int i = 0; i < effectiveSegments; ++i) {
                float angle1 = static_cast<float>(i) * angleStep;
                float angle2 = static_cast<float>(i + 1) * angleStep;

                // Calculate distortion offset using sine wave
                // This creates a waviness that appears to travel around the circle
                float distort1 = std::sin(angle1 * distortFrequency + distortTime) * distortAmplitude;
                float distort2 = std::sin(angle2 * distortFrequency + distortTime) * distortAmplitude;

                // Apply distortion by varying the radius at each point
                float distortedRadius1 = radius + distort1;
                float distortedRadius2 = radius + distort2;

                int x1 = static_cast<int>(centerX + distortedRadius1 * std::cos(angle1));
                int y1 = static_cast<int>(centerY + distortedRadius1 * std::sin(angle1));
                int x2 = static_cast<int>(centerX + distortedRadius2 * std::cos(angle2));
                int y2 = static_cast<int>(centerY + distortedRadius2 * std::sin(angle2));

                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        } else {
            // Thick ring with distortion
            for (int i = 0; i < effectiveSegments; ++i) {
                float angle1 = static_cast<float>(i) * angleStep;
                float angle2 = static_cast<float>(i + 1) * angleStep;

                float cos1 = std::cos(angle1);
                float sin1 = std::sin(angle1);
                float cos2 = std::cos(angle2);
                float sin2 = std::sin(angle2);

                // Calculate distortion for this segment
                float distort1 = std::sin(angle1 * distortFrequency + distortTime) * distortAmplitude;
                float distort2 = std::sin(angle2 * distortFrequency + distortTime) * distortAmplitude;

                // Apply distortion to both inner and outer radius
                float distortedInnerRadius1 = innerRadius + distort1;
                float distortedOuterRadius1 = outerRadius + distort1;
                float distortedInnerRadius2 = innerRadius + distort2;
                float distortedOuterRadius2 = outerRadius + distort2;

                // Inner and outer points for this segment with distortion
                float innerX1 = centerX + distortedInnerRadius1 * cos1;
                float innerY1 = centerY + distortedInnerRadius1 * sin1;
                float outerX1 = centerX + distortedOuterRadius1 * cos1;
                float outerY1 = centerY + distortedOuterRadius1 * sin1;
                float innerX2 = centerX + distortedInnerRadius2 * cos2;
                float innerY2 = centerY + distortedInnerRadius2 * sin2;
                float outerX2 = centerX + distortedOuterRadius2 * cos2;
                float outerY2 = centerY + distortedOuterRadius2 * sin2;

                // Draw lines to create thickness
                int numThicknessLines = static_cast<int>(thickness / 1.0f) + 1;
                for (int t = 0; t <= numThicknessLines; ++t) {
                    float tFactor = static_cast<float>(t) / static_cast<float>(numThicknessLines);
                    float currentInnerRadius = distortedInnerRadius1 + tFactor * (distortedOuterRadius1 - distortedInnerRadius1);
                    float currentInnerRadius2 = distortedInnerRadius2 + tFactor * (distortedOuterRadius2 - distortedInnerRadius2);
                    
                    int x1 = static_cast<int>(centerX + currentInnerRadius * cos1);
                    int y1 = static_cast<int>(centerY + currentInnerRadius * sin1);
                    int x2 = static_cast<int>(centerX + currentInnerRadius2 * cos2);
                    int y2 = static_cast<int>(centerY + currentInnerRadius2 * sin2);
                    
                    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
                }
            }
        }

        // Restore previous renderer state
        SDL_SetRenderDrawBlendMode(renderer, previousBlendMode);
        SDL_SetRenderDrawColor(renderer, prevR, prevG, prevB, prevA);
    }

    // =========================================================================
    // LIGHTNING EFFECT IMPLEMENTATION
    // =========================================================================

    std::size_t VFXSystem::GetFreeLightningIndex() {
        for (std::size_t i = 0; i < m_Lightnings.size(); ++i) {
            if (!m_Lightnings[i].active) {
                return i;
            }
        }

        if (m_Lightnings.size() < m_MaxLightnings) {
            m_Lightnings.push_back(LightningEffect{});
            return m_Lightnings.size() - 1;
        }

        return m_MaxLightnings;
    }

    int VFXSystem::CreateLightning(float startX, float startY, float endX, float endY, float lifetime) {
        std::size_t index = GetFreeLightningIndex();
        if (index >= m_MaxLightnings) {
            return -1;
        }

        if (index >= m_Lightnings.size()) {
            m_Lightnings.push_back(LightningEffect{});
        }

        LightningEffect& lightning = m_Lightnings[index];
        lightning.active = true;
        lightning.x1 = startX;
        lightning.y1 = startY;
        lightning.x2 = endX;
        lightning.y2 = endY;
        
        // Calculate distance for auto-scaling
        float dx = endX - startX;
        float dy = endY - startY;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        // Auto-scale jaggedness (10% of line length)
        lightning.jaggedness = 0.1f;
        
        // Auto-scale segment count (1 segment per 30 pixels, minimum 6)
        int autoSegments = static_cast<int>(distance / 30.0f);
        lightning.segments = autoSegments < 6 ? 6 : autoSegments;
        
        lightning.thickness = 2.0f;
        lightning.r = 100;
        lightning.g = 180;
        lightning.b = 255;
        lightning.a = 255;
        lightning.lifetime = lifetime;
        lightning.timeAlive = 0.0f;
        lightning.fadeOut = true;
        lightning.flickerEnabled = true;
        lightning.flickerSpeed = 0.05f;
        lightning.flickerTimer = 0.0f;
        lightning.renderLayer = 0;
        lightning.zOrder = 0;
        
        // Generate initial path
        GenerateLightningPath(lightning);

        return static_cast<int>(index);
    }

    void VFXSystem::DestroyLightning(int id) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size()) {
            return;
        }
        m_Lightnings[id].active = false;
        m_Lightnings[id].pathX.clear();
        m_Lightnings[id].pathY.clear();
    }

    void VFXSystem::SetLightningPosition(int id, float startX, float startY, float endX, float endY) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        
        LightningEffect& lightning = m_Lightnings[id];
        lightning.x1 = startX;
        lightning.y1 = startY;
        lightning.x2 = endX;
        lightning.y2 = endY;
        
        // Regenerate path with new positions
        GenerateLightningPath(lightning);
    }

    void VFXSystem::SetLightningColor(int id, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        m_Lightnings[id].r = r;
        m_Lightnings[id].g = g;
        m_Lightnings[id].b = b;
        m_Lightnings[id].a = a;
    }

    void VFXSystem::SetLightningProperties(int id, float thickness, float jaggedness, int segments) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        m_Lightnings[id].thickness = thickness;
        m_Lightnings[id].jaggedness = jaggedness;
        m_Lightnings[id].segments = segments > 2 ? segments : 2;
        // Regenerate path with new properties
        GenerateLightningPath(m_Lightnings[id]);
    }

    void VFXSystem::SetLightningFlicker(int id, bool enabled, float speed) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        m_Lightnings[id].flickerEnabled = enabled;
        m_Lightnings[id].flickerSpeed = speed;
        m_Lightnings[id].flickerTimer = 0.0f;
    }

    void VFXSystem::SetLightningRenderLayer(int id, int layer, int zOrder) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        m_Lightnings[id].renderLayer = layer;
        m_Lightnings[id].zOrder = zOrder;
    }

    void VFXSystem::SetLightningFadeOut(int id, bool enabled) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        m_Lightnings[id].fadeOut = enabled;
    }

    bool VFXSystem::IsLightningActive(int id) const {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size()) {
            return false;
        }
        return m_Lightnings[id].active;
    }

    void VFXSystem::SetLightningCorruptionEnabled(int id, bool enabled) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        LightningEffect& lightning = m_Lightnings[id];
        lightning.corruptionEnabled = enabled;
        
        // Initialize segment corruption array if enabling
        if (enabled && lightning.segmentCorruption.size() != static_cast<std::size_t>(lightning.segments)) {
            lightning.segmentCorruption.clear();
            lightning.segmentCorruption.resize(lightning.segments, 0.0f);
        }
    }

    void VFXSystem::SetLightningCorruptionColor(int id, Uint8 r, Uint8 g, Uint8 b) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        m_Lightnings[id].corruptR = r;
        m_Lightnings[id].corruptG = g;
        m_Lightnings[id].corruptB = b;
    }

    void VFXSystem::SetLightningCorruptionRates(int id, float decayRate, float spreadRate) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        m_Lightnings[id].corruptionDecayRate = decayRate;
        m_Lightnings[id].corruptionSpreadRate = spreadRate;
    }

    void VFXSystem::ApplyLightningCorruption(int id, int segmentIndex, float amount) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        LightningEffect& lightning = m_Lightnings[id];
        if (!lightning.corruptionEnabled) {
            return;
        }
        
        // Ensure corruption array is properly sized
        if (lightning.segmentCorruption.size() != static_cast<std::size_t>(lightning.segments)) {
            lightning.segmentCorruption.resize(lightning.segments, 0.0f);
        }
        
        if (segmentIndex >= 0 && segmentIndex < lightning.segments) {
            float previousCorruption = lightning.segmentCorruption[segmentIndex];
            lightning.segmentCorruption[segmentIndex] += amount;
            
            // If segment is fully corrupted, overflow to neighbors
            if (lightning.segmentCorruption[segmentIndex] > 1.0f) {
                float overflow = lightning.segmentCorruption[segmentIndex] - 1.0f;
                lightning.segmentCorruption[segmentIndex] = 1.0f;
                
                // Distribute overflow to adjacent segments (50% each if both exist)
                int leftNeighbor = segmentIndex - 1;
                int rightNeighbor = segmentIndex + 1;
                bool hasLeft = leftNeighbor >= 0;
                bool hasRight = rightNeighbor < lightning.segments;
                
                if (hasLeft && hasRight) {
                    // Split overflow between both neighbors
                    lightning.segmentCorruption[leftNeighbor] += overflow * 0.5f;
                    lightning.segmentCorruption[rightNeighbor] += overflow * 0.5f;
                    
                    // Clamp neighbors
                    if (lightning.segmentCorruption[leftNeighbor] > 1.0f) {
                        lightning.segmentCorruption[leftNeighbor] = 1.0f;
                    }
                    if (lightning.segmentCorruption[rightNeighbor] > 1.0f) {
                        lightning.segmentCorruption[rightNeighbor] = 1.0f;
                    }
                } else if (hasLeft) {
                    // Only left neighbor exists, give it all overflow
                    lightning.segmentCorruption[leftNeighbor] += overflow;
                    if (lightning.segmentCorruption[leftNeighbor] > 1.0f) {
                        lightning.segmentCorruption[leftNeighbor] = 1.0f;
                    }
                } else if (hasRight) {
                    // Only right neighbor exists, give it all overflow
                    lightning.segmentCorruption[rightNeighbor] += overflow;
                    if (lightning.segmentCorruption[rightNeighbor] > 1.0f) {
                        lightning.segmentCorruption[rightNeighbor] = 1.0f;
                    }
                }
            }
        }
    }

    void VFXSystem::ApplyLightningCorruptionAtPoint(int id, float x, float y, float amount) {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return;
        }
        LightningEffect& lightning = m_Lightnings[id];
        if (!lightning.corruptionEnabled || lightning.pathX.size() < 2) {
            return;
        }
        
        // Find the closest segment to the given point
        int closestSegment = 0;
        float closestDist = std::numeric_limits<float>::max();
        
        for (std::size_t i = 0; i < lightning.pathX.size() - 1; ++i) {
            // Get segment midpoint
            float midX = (lightning.pathX[i] + lightning.pathX[i + 1]) * 0.5f;
            float midY = (lightning.pathY[i] + lightning.pathY[i + 1]) * 0.5f;
            
            float dx = x - midX;
            float dy = y - midY;
            float dist = dx * dx + dy * dy;
            
            if (dist < closestDist) {
                closestDist = dist;
                closestSegment = static_cast<int>(i);
            }
        }
        
        ApplyLightningCorruption(id, closestSegment, amount);
    }

    float VFXSystem::GetLightningTotalCorruption(int id) const {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return 0.0f;
        }
        const LightningEffect& lightning = m_Lightnings[id];
        if (!lightning.corruptionEnabled || lightning.segmentCorruption.empty()) {
            return 0.0f;
        }
        
        float total = 0.0f;
        for (float corruption : lightning.segmentCorruption) {
            total += corruption;
        }
        return total / static_cast<float>(lightning.segmentCorruption.size());
    }

    float VFXSystem::GetLightningSegmentCorruption(int id, int segmentIndex) const {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return 0.0f;
        }
        const LightningEffect& lightning = m_Lightnings[id];
        if (segmentIndex < 0 || static_cast<std::size_t>(segmentIndex) >= lightning.segmentCorruption.size()) {
            return 0.0f;
        }
        return lightning.segmentCorruption[segmentIndex];
    }

    int VFXSystem::GetLightningSegmentCount(int id) const {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return 0;
        }
        return m_Lightnings[id].segments;
    }

    bool VFXSystem::GetLightningSegmentPosition(int id, int segmentIndex, float& outX, float& outY) const {
        if (id < 0 || static_cast<std::size_t>(id) >= m_Lightnings.size() || !m_Lightnings[id].active) {
            return false;
        }
        const LightningEffect& lightning = m_Lightnings[id];
        if (segmentIndex < 0 || static_cast<std::size_t>(segmentIndex) >= lightning.pathX.size() - 1) {
            return false;
        }
        
        // Return midpoint of segment
        outX = (lightning.pathX[segmentIndex] + lightning.pathX[segmentIndex + 1]) * 0.5f;
        outY = (lightning.pathY[segmentIndex] + lightning.pathY[segmentIndex + 1]) * 0.5f;
        return true;
    }

    void VFXSystem::GenerateLightningPath(LightningEffect& lightning) {
        lightning.pathX.clear();
        lightning.pathY.clear();
        
        int numPoints = lightning.segments + 1;
        lightning.pathX.reserve(numPoints);
        lightning.pathY.reserve(numPoints);
        
        float dx = lightning.x2 - lightning.x1;
        float dy = lightning.y2 - lightning.y1;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance < 0.001f) {
            // Points are too close, just draw a point
            lightning.pathX.push_back(lightning.x1);
            lightning.pathY.push_back(lightning.y1);
            lightning.pathX.push_back(lightning.x2);
            lightning.pathY.push_back(lightning.y2);
            return;
        }
        
        // Calculate perpendicular direction (normalized)
        float perpX = -dy / distance;
        float perpY = dx / distance;
        
        // Maximum perpendicular offset based on jaggedness and distance
        float maxOffset = distance * lightning.jaggedness;
        
        for (int i = 0; i < numPoints; ++i) {
            float t = static_cast<float>(i) / static_cast<float>(lightning.segments);
            
            // Base position along the line
            float baseX = lightning.x1 + dx * t;
            float baseY = lightning.y1 + dy * t;
            
            // Apply random perpendicular offset, tapering at endpoints using sin(t * PI)
            float taper = std::sin(t * static_cast<float>(M_PI));
            float randomOffset = (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 2.0f;
            float offset = randomOffset * maxOffset * taper;
            
            lightning.pathX.push_back(baseX + perpX * offset);
            lightning.pathY.push_back(baseY + perpY * offset);
        }
    }

    void VFXSystem::DrawLightning(SDL_Renderer* renderer, const LightningEffect& lightning) {
        if (lightning.pathX.size() < 2) {
            return;
        }
        
        // Calculate alpha based on lifetime fade
        Uint8 finalAlpha = lightning.a;
        if (lightning.fadeOut && lightning.lifetime > 0.0f) {
            float lifetimeRatio = 1.0f - (lightning.timeAlive / lightning.lifetime);
            finalAlpha = static_cast<Uint8>(lightning.a * lifetimeRatio);
        }
        
        // Draw each segment of the lightning bolt
        for (std::size_t i = 0; i < lightning.pathX.size() - 1; ++i) {
            // If corruption is enabled, we use a more complex rendering method
            // to achieve the "Venn diagram" circle-shaped leak effect while preserving the line shape.
            if (lightning.corruptionEnabled) {
                float x1 = lightning.pathX[i];
                float y1 = lightning.pathY[i];
                float x2 = lightning.pathX[i+1];
                float y2 = lightning.pathY[i+1];
                
                float dx = x2 - x1;
                float dy = y2 - y1;
                float len = std::sqrt(dx*dx + dy*dy);
                
                // Subdivide the segment into small steps to create a smooth gradient
                // Step size of ~2-4 pixels
                int steps = static_cast<int>(len / 3.0f);
                if (steps < 1) steps = 1;
                
                for (int s = 0; s < steps; ++s) {
                    float t1 = static_cast<float>(s) / steps;
                    float t2 = static_cast<float>(s + 1) / steps;
                    
                    float subX1 = x1 + dx * t1;
                    float subY1 = y1 + dy * t1;
                    float subX2 = x1 + dx * t2;
                    float subY2 = y1 + dy * t2;
                    
                    float midX = (subX1 + subX2) * 0.5f;
                    float midY = (subY1 + subY2) * 0.5f;
                    
                    // Calculate corruption at this point based on overlapping circles
                    float totalCorruption = 0.0f;
                    
                    // Check influence from current, previous, and next segments
                    // We treat each segment as having a "corruption circle" at its center
                    for (int k = static_cast<int>(i) - 1; k <= static_cast<int>(i) + 1; ++k) {
                        if (k >= 0 && k < static_cast<int>(lightning.pathX.size()) - 1 && k < static_cast<int>(lightning.segmentCorruption.size())) {
                            float cVal = lightning.segmentCorruption[k];
                            if (cVal > 0.0f) {
                                // Get center of segment k
                                float kX1 = lightning.pathX[k];
                                float kY1 = lightning.pathY[k];
                                float kX2 = lightning.pathX[k+1];
                                float kY2 = lightning.pathY[k+1];
                                float kMidX = (kX1 + kX2) * 0.5f;
                                float kMidY = (kY1 + kY2) * 0.5f;
                                
                                // Calculate distance from current point to segment k center
                                float distSq = (midX - kMidX)*(midX - kMidX) + (midY - kMidY)*(midY - kMidY);
                                float dist = std::sqrt(distSq);
                                
                                // Radius of influence (overlap factor)
                                float kLen = std::sqrt((kX2-kX1)*(kX2-kX1) + (kY2-kY1)*(kY2-kY1));
                                float radius = kLen * 1.5f; // 1.5 overlap factor for wider spread (Venn diagram style)
                                
                                if (dist < radius) {
                                    // Radial falloff (circle shape)
                                    float influence = 1.0f - (dist / radius);
                                    // Smooth it a bit
                                    influence = influence * influence * (3.0f - 2.0f * influence);
                                    totalCorruption += cVal * influence;
                                }
                            }
                        }
                    }
                    
                    if (totalCorruption > 1.0f) totalCorruption = 1.0f;
                    
                    // Calculate color for this sub-segment
                    Uint8 r = lightning.r;
                    Uint8 g = lightning.g;
                    Uint8 b = lightning.b;
                    
                    if (totalCorruption > 0.0f) {
                        r = static_cast<Uint8>(lightning.r + (lightning.corruptR - lightning.r) * totalCorruption);
                        g = static_cast<Uint8>(lightning.g + (lightning.corruptG - lightning.g) * totalCorruption);
                        b = static_cast<Uint8>(lightning.b + (lightning.corruptB - lightning.b) * totalCorruption);
                    }
                    
                    DrawThickLine(renderer, subX1, subY1, subX2, subY2, lightning.thickness, r, g, b, finalAlpha);
                }
            } else {
                // Standard rendering for non-corrupted lightning
                DrawThickLine(renderer, 
                             lightning.pathX[i], lightning.pathY[i],
                             lightning.pathX[i + 1], lightning.pathY[i + 1],
                             lightning.thickness,
                             lightning.r, lightning.g, lightning.b, finalAlpha);
            }
        }
    }

    void VFXSystem::DrawThickLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2,
                                  float thickness, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        // Save current renderer state
        SDL_BlendMode previousBlendMode;
        SDL_GetRenderDrawBlendMode(renderer, &previousBlendMode);
        Uint8 prevR, prevG, prevB, prevA;
        SDL_GetRenderDrawColor(renderer, &prevR, &prevG, &prevB, &prevA);

        // Enable blending for alpha
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
        
        if (thickness <= 1.0f) {
            // Simple single-pixel line
            SDL_RenderDrawLine(renderer, 
                              static_cast<int>(x1), static_cast<int>(y1),
                              static_cast<int>(x2), static_cast<int>(y2));
        } else {
            // Calculate line direction and perpendicular
            float dx = x2 - x1;
            float dy = y2 - y1;
            float length = std::sqrt(dx * dx + dy * dy);
            
            if (length < 0.001f) {
                // Points are the same, draw a point
                SDL_RenderDrawPoint(renderer, static_cast<int>(x1), static_cast<int>(y1));
            } else {
                // Normalize and get perpendicular
                float perpX = -dy / length * thickness * 0.5f;
                float perpY = dx / length * thickness * 0.5f;
                
                // Draw multiple parallel lines to create thickness
                int numLines = static_cast<int>(thickness) + 1;
                for (int i = 0; i <= numLines; ++i) {
                    float t = static_cast<float>(i) / static_cast<float>(numLines) - 0.5f;
                    float offsetX = perpX * t * 2.0f;
                    float offsetY = perpY * t * 2.0f;
                    
                    SDL_RenderDrawLine(renderer,
                                      static_cast<int>(x1 + offsetX), static_cast<int>(y1 + offsetY),
                                      static_cast<int>(x2 + offsetX), static_cast<int>(y2 + offsetY));
                }
            }
        }

        // Restore previous renderer state
        SDL_SetRenderDrawBlendMode(renderer, previousBlendMode);
        SDL_SetRenderDrawColor(renderer, prevR, prevG, prevB, prevA);
    }
}
