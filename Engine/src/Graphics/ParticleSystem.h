#pragma once

#include <vector>
#include <cstdint>

#include "SDL.h"
#include "Architecture/ECS/Component.hpp"

namespace IonixEngine {

    struct Particle {
        bool active = false;
        SDL_Texture* texture = nullptr;
        int srcWidth = 0;
        int srcHeight = 0;

        float x = 0.0f;
        float y = 0.0f;
        float vx = 0.0f;
        float vy = 0.0f;
        float ax = 0.0f;
        float ay = 0.0f;
        float age = 0.0f;
        float lifetime = 0.0f;
        float drag = 0.0f;

        float startSize = 0.0f;
        float endSize = 0.0f;

        Uint8 startR = 255;
        Uint8 startG = 255;
        Uint8 startB = 255;
        Uint8 startA = 255;

        Uint8 endR = 255;
        Uint8 endG = 255;
        Uint8 endB = 255;
        Uint8 endA = 0;

        int renderLayer = 0;
        int zOrder = 0;
        bool useRainbow = false;
    };

    class ParticleSystem {
    public:
        ParticleSystem();

        void Init();
        void Shutdown();

        void Update(float dt);
        void Render(RenderData* renderData);

        void Clear();

        void Emit(
            uint32_t textureHash,
            int renderLayer,
            float x,
            float y,
            float vx,
            float vy,
            float lifetime,
            float startSize,
            float endSize,
            Uint8 startR,
            Uint8 startG,
            Uint8 startB,
            Uint8 startA,
            Uint8 endR,
            Uint8 endG,
            Uint8 endB,
            Uint8 endA,
            int zOrder = 0,
            float gravityX = 0.0f,
            float gravityY = 0.0f,
            float drag = 0.0f,
            bool useRainbow = false
        );

    private:
        std::vector<Particle> m_Particles;
        std::size_t m_MaxParticles;

        std::size_t GetFreeIndex();
    };
}
