#include "Component.hpp"

namespace IonixEngine {

Component::Component(Entity* entity, bool hasPhysics, bool canRender, bool tag) : 
    entity(entity), 
    hasPhysics(hasPhysics), 
    canRender(canRender),
    isTag(tag)
{}

enum Layer {
    Default = 1 << 0,  // 0001
    UI = 1 << 1,       // 0010
    Environment = 1 << 2, // 0100
    Player = 1 << 3,   // 1000
    // Add more layers as needed
};

void Component::Start() {}
void Component::Update(float dt) {}
void Component::Render(RenderData* renderData) {}
void Component::Collide(Entity* other) {}
void Component::Destroy() {}

}
