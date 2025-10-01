#include "Entity.hpp"
#include "Component.hpp"
#include <cstdio>

class ConstantLogger : public Component {
public:
    ConstantLogger(Entity* entity) : Component(entity, false, false, false) {}

    virtual void Update(float dt) override;
};

class SingleLogger : public Component { 
public:
    SingleLogger(Entity* entity) : Component(entity, false, false, false) {}
    virtual void Start() override;
    void ForceLog();
};

int main(void) {
    std::vector<Entity> entities;

    Entity single = Entity(Vec2 { 100, 100 });
    Entity constant = Entity(Vec2 { 10, 10 });

    single.AddComponent<SingleLogger>(new SingleLogger(&single));
    constant.AddComponent<ConstantLogger>(new ConstantLogger(&constant));

    entities.push_back(single);
    entities.push_back(constant);

    size_t count = 0;
    for (float dt = 0.0f; dt < 1.0f; dt += 0.1f) {
        for (Entity& entity : entities) {
            entity.Update(dt);
            SingleLogger* logger;
            if (entity.TryGetComponent<SingleLogger>(logger) && count == 4) {
                logger->ForceLog();
            }
        }
        count++;
    }
}

void ConstantLogger::Update(float dt) {
    printf("[Constant Logger] DeltaTime is %.1f\n", dt);
}

void SingleLogger::Start() {
    printf("[Single Logger] Started...\n");
}

void SingleLogger::ForceLog() {
    printf("[Single Logger] Forced Log...\n");
}
