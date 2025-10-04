#include "Component.hpp"

Component::Component(Entity* entity, bool hasPhysics, bool canRender, bool tag) : 
    entity(entity), 
    hasPhysics(hasPhysics), 
    canRender(canRender),
    isTag(tag)
{}

void Component::Start() {}
void Component::Update(float dt) {}
void Component::Render(RenderData* renderData) {}
void Component::Collide(Entity* other) {}
void Component::Destroy() {}
