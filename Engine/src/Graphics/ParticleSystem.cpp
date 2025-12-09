#include "Graphics/ParticleSystem.h"

#include "Architecture/TextureManager/TextureManager.h"
#include "Graphics/QueueRenderer.h"

namespace IonixEngine {

    ParticleSystem::ParticleSystem()
        : m_MaxParticles(1024) {
    }

    void ParticleSystem::Init() {
        m_Particles.clear();
        m_Particles.reserve(m_MaxParticles);
    }

    void ParticleSystem::Shutdown() {
        Clear();
    }

    void ParticleSystem::Clear() {
        m_Particles.clear();
    }

    std::size_t ParticleSystem::GetFreeIndex() {
        for (std::size_t i = 0; i < m_Particles.size(); ++i) {
            if (!m_Particles[i].active) {
                return i;
            }
        }

        if (m_Particles.size() < m_MaxParticles) {
            m_Particles.push_back(Particle{});
            return m_Particles.size() - 1;
        }

        return m_MaxParticles;
    }

    void ParticleSystem::Emit(
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
        int zOrder,
        float gravityX,
        float gravityY,
        float drag
    ) {
        std::size_t index = GetFreeIndex();
        if (index >= m_Particles.size()) {
            return;
        }

        SDL_Texture* texture = TextureManager::Get().GetTexture(textureHash).GetTexture();
        if (!texture) {
            return;
        }

        int srcW = 0;
        int srcH = 0;
        SDL_QueryTexture(texture, nullptr, nullptr, &srcW, &srcH);

        Particle& p = m_Particles[index];
        p.active = true;
        p.texture = texture;
        p.srcWidth = srcW;
        p.srcHeight = srcH;
        p.x = x;
        p.y = y;
        p.vx = vx;
        p.vy = vy;
        p.ax = gravityX;
        p.ay = gravityY;
        p.age = 0.0f;
        p.lifetime = lifetime;
        p.drag = drag;
        p.startSize = startSize;
        p.endSize = endSize;
        p.startR = startR;
        p.startG = startG;
        p.startB = startB;
        p.startA = startA;
        p.endR = endR;
        p.endG = endG;
        p.endB = endB;
        p.endA = endA;
        p.renderLayer = renderLayer;
        p.zOrder = zOrder;
    }

    void ParticleSystem::Update(float dt) {
        for (std::size_t i = 0; i < m_Particles.size(); ++i) {
            Particle& p = m_Particles[i];
            if (!p.active) {
                continue;
            }

            p.age += dt;
            if (p.age >= p.lifetime || p.lifetime <= 0.0f) {
                p.active = false;
                continue;
            }

            // Apply simple acceleration (e.g., gravity)
            p.vx += p.ax * dt;
            p.vy += p.ay * dt;

            // Apply simple drag as a damping factor on velocity
            if (p.drag > 0.0f) {
                float damping = 1.0f - p.drag * dt;
                if (damping < 0.0f) damping = 0.0f;
                p.vx *= damping;
                p.vy *= damping;
            }

            p.x += p.vx * dt;
            p.y += p.vy * dt;
        }
    }

    void ParticleSystem::Render(RenderData* renderData) {
        if (!renderData || !renderData->queue) {
            return;
        }

        for (std::size_t i = 0; i < m_Particles.size(); ++i) {
            Particle& p = m_Particles[i];
            if (!p.active || !p.texture) {
                continue;
            }

            float t = (p.lifetime > 0.0f) ? (p.age / p.lifetime) : 1.0f;
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;

            float size = p.startSize + (p.endSize - p.startSize) * t;
            if (size < 0.0f) size = 0.0f;

            int destSize = static_cast<int>(size + 0.5f);
            int half = destSize / 2;

            SDL_Rect dest{
                static_cast<int>(p.x) - half,
                static_cast<int>(p.y) - half,
                destSize,
                destSize
            };

            SDL_Rect src{ 0, 0, p.srcWidth, p.srcHeight };

            float h = t * 6.0f;
            int sector = static_cast<int>(h);
            float f = h - static_cast<float>(sector);
            float q = 1.0f - f;
            float rf = 1.0f;
            float gf = 0.0f;
            float bf = 0.0f;
            switch (sector % 6) {
            case 0: rf = 1.0f; gf = f;    bf = 0.0f; break;
            case 1: rf = q;    gf = 1.0f;  bf = 0.0f; break;
            case 2: rf = 0.0f; gf = 1.0f;  bf = f;    break;
            case 3: rf = 0.0f; gf = q;     bf = 1.0f; break;
            case 4: rf = f;    gf = 0.0f;  bf = 1.0f; break;
            case 5: rf = 1.0f; gf = 0.0f;  bf = q;    break;
            }
            int r = static_cast<int>(rf * 255.0f);
            int g = static_cast<int>(gf * 255.0f);
            int b = static_cast<int>(bf * 255.0f);

            if (r < 0) r = 0; if (r > 255) r = 255;
            if (g < 0) g = 0; if (g > 255) g = 255;
            if (b < 0) b = 0; if (b > 255) b = 255;

            Uint8 finalR = static_cast<Uint8>(r);
            Uint8 finalG = static_cast<Uint8>(g);
            Uint8 finalB = static_cast<Uint8>(b);
            float alphaT = t * t * t;
            int a = static_cast<int>(p.startA) + static_cast<int>((static_cast<int>(p.endA) - static_cast<int>(p.startA)) * alphaT);
            if (a < 0) a = 0; if (a > 255) a = 255;
            Uint8 finalA = static_cast<Uint8>(a);

            RenderCall call{
                p.texture,
                dest,
                src,
                p.zOrder,
                0.0,
                finalR,
                finalG,
                finalB,
                finalA,
                0.0f,
                p.renderLayer
            };

            renderData->queue->AddToQueue(call);
        }
    }
}
