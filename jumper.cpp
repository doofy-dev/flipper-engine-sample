#include <furi.h>
#include "flipper-game-engine/game-engine.h"
#include "jumper_icons.h"


class RotationComponent : public Component<RotationComponent> {
    float speed = 1;
public:
    explicit RotationComponent(float s) : speed(s) {}

    RotationComponent() : speed(1) {}

    void Update(const float &delta) override {
        entity->get_transform()->rotate(speed * (float) delta);
    }

    void Swap() {
        speed *= -1;
    }
};

class ScaleComponent : public Component<ScaleComponent> {
    float scale = 1;
public:
    void OnInput(InputKey key, InputState type) override {
        if (key == InputKeyUp && type == InputPress) {
            scale += 0.5f;
            entity->get_transform()->set_scale(scale);
        } else if (key == InputKeyDown && type == InputPress) {
            scale -= 0.5f;
            entity->get_transform()->set_scale(scale);
        }
    }
};

class SwapRotationComponent : public Component<SwapRotationComponent> {
    RotationComponent *component = nullptr;
public:
    void Start() override {
        component = entity->GetComponent<RotationComponent>();
    }

    void OnInput(InputKey key, InputState type) override {
        if (key == InputKeyOk && type == InputPress)
            component->Swap();
    }
};


class CheckHeapComponent : public Component<CheckHeapComponent> {
public:
    void OnInput(InputKey key, InputState type) override {
        if (key == InputKeyUp && type == InputPress)
            CHECK_HEAP();
    }
};


#ifdef __cplusplus
extern "C"
{
#endif

int32_t jumper_app(void *p) {
    UNUSED(p);
    size_t heap = memmgr_get_free_heap();
    auto *engine_instance = new Engine("Jumper", 30, true);

    auto *current_scene = new Scene("play");

    auto *ball = new Entity("Ball");
    ball->AddComponent<RotationComponent>();
    ball->AddComponent<CheckHeapComponent>();
    ball->set_sprite(&I_platforms);
    ball->get_transform()->set_position({20, 32});
    current_scene->Add(ball);

    ball = new Entity("Ball 2");
    ball->AddComponent<RotationComponent>();
    ball->AddComponent<ScaleComponent>();
    ball->set_sprite(&I_big_ball);
    ball->get_transform()->set_position({64, 16});
    current_scene->Add(ball);

    ball = new Entity("Ball 3");
    ball->AddComponent<RotationComponent>(0.5f);
    ball->AddComponent<SwapRotationComponent>();
    ball->set_sprite(&I_platforms);
//    ball->AddComponent<PhysicsBody>((Vector){1,0}, 1, Material_CONCRETE, false);
    ball->get_transform()->set_position({100, 32});
    current_scene->Add(ball);

    engine_instance->SetScene(current_scene);


    engine_instance->Start();
    delete engine_instance;

    FURI_LOG_D("GAME", "diff %d", memmgr_get_free_heap() - heap);

    return 1;
}

#ifdef __cplusplus
}
#endif