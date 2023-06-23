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
        component = entity->get_component<RotationComponent>();
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

    Vector corners[4]= {
            {-64,-3.5f},
            {64,-3.5f},
            {64,3.5f},
            {-64,3.5f},
    };

    auto *current_scene = new Scene("play");

    auto *entity = new Entity("Platform1");
    entity->set_sprite(&I_platforms);
    entity->get_transform()->set_position({64, 60});
    entity->add_component<PhysicsBody>((Vector){0, 9.8}, 10, Material_RUBBER, true);
    entity->add_component<PolyCollider>(corners, 4);
//    entity->add_component<RotationComponent>();
    current_scene->Add(entity);

    entity = new Entity("Ball");
    entity->set_sprite(&I_big_ball);
    entity->add_component<PhysicsBody>((Vector) {0,  9.8}, 0.1, Material_RUBBER, false);
    entity->add_component<CircleCollider>(8);
    entity->get_transform()->set_position({64, 16});
//    entity->add_component<RotationComponent>();
    current_scene->Add(entity);
    entity->get_component<PhysicsBody>()->velocity = {0, 10};
/*

    entity = new Entity("Platform2");
    entity->set_sprite(&I_platforms);
    entity->get_transform()->rotate_deg(-45);
    entity->get_transform()->set_position({110, 32});
    entity->add_component<PhysicsBody>((Vector){0, 9.8}, 10, Material_RUBBER, true);
    entity->add_component<PolyCollider>(corners, 4);
//    entity->add_component<RotationComponent>();
    current_scene->Add(entity);

    entity = new Entity("Platform3");
    entity->set_sprite(&I_platforms);
    entity->get_transform()->rotate_deg(45);
    entity->add_component<PhysicsBody>((Vector){0, 9.8}, 10, Material_RUBBER, true);
    entity->add_component<PolyCollider>(corners, 4);
    entity->get_transform()->set_position({10, 32});
//    entity->add_component<RotationComponent>();
    current_scene->Add(entity);
*/


    engine_instance->SetScene(current_scene);


    engine_instance->Start();
    delete engine_instance;

    FURI_LOG_D("GAME", "diff %d", memmgr_get_free_heap() - heap);

    return 1;
}

#ifdef __cplusplus
}
#endif