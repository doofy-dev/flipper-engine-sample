#include <furi.h>
#include "flipper-game-engine/game_engine.h"
#include "defines.h"
#include "jumper_icons.h"
#include "flipper-game-engine/util/util.h"
#include "flipper-game-engine/physics/physics.h"


typedef struct {
    int range;
} ball_data;

void init_ball(ComponentInfo *component, void *state) {
    UNUSED(state);
    ball_data *data = (ball_data *) component->data;
    data->range = 10;
}

void rotate(ComponentInfo *component, void *state) {
    UNUSED(state);
    add_rotation(&(component->entity->transform), (float) 0.05);
}
void log_pos(ComponentInfo *component, void *state) {
    UNUSED(state);
    Vector p;
    get_matrix_translation(&(component->entity->transform.modelMatrix), &p);
    FURI_LOG_D("Ball pos", "%f %f", (double)p.x, (double) p.y);
}

void scale(ComponentInfo *component, void *state) {
    UNUSED(state);
    add_scale(&(component->entity->transform), 0.1);
    if (component->entity->transform.scale > 4)
        set_scale(&(component->entity->transform), 0.1);
}

void setup_play_scene() {
    Scene *s = new_scene("Play");


    entity_t *e = new_entity("Ball");
    e->transform.position = (Vector) {20, 0};
    add_sprite(e, &I_small_ball, (Vector) {0.5, 0.5});
    PhysicsBody *body = new_physics_body((Vector) {0, 0.00098f}, 0.1f, Material_RUBBER, false);
//    PhysicsBody *body = new_physics_body((Vector) {0, 0.0005f},0.1, Material_RUBBER, false);
    set_to_circle_collider(body, 4);
    add_physics_body(e, body);
    add_to_scene(s, e);
    add_component(e, NULL, log_pos, 0);

    entity_t *e2 = new_entity("Ball2");
    e2->transform.position = (Vector) {25, 30};
    add_sprite(e2, &I_small_ball, (Vector) {0.5, 0.5});
    body = new_physics_body((Vector) {0, 0}, 1, Material_RUBBER, false);
    set_to_circle_collider(body, 4);
    add_physics_body(e2, body);
    add_to_scene(s, e2);

/*

    entity_t *e3 = new_entity("Platform");
    e3->transform.position = (Vector) {0, 50};
    e3->transform.rotation = M_PI_2 / 4;
    add_sprite(e3, &I_platforms, (Vector) {0, 0});
//    e3->transform.rotation=0.7853982;
    body = new_physics_body((Vector) {0, 0}, 1, Material_CONCRETE, true);
    set_to_polygon_collider(body, (Vector[4]) {
            {0,   0},
            {128, 0},
            {128, 7},
            {0,   7},
    }, 4);
    add_physics_body(e3, body);
    add_to_scene(s, e3);
*/


    entity_t *e4 = new_entity("Line");
    e4->transform.position = (Vector) {0, 48};
    body = new_physics_body((Vector) {0, 0}, 1, Material_CONCRETE, true);
    set_to_polygon_collider(body, (Vector[2]) {
            {0, 0},
            {128, 0}
    }, 2);
    add_physics_body(e4,body);
    add_to_scene(s, e4);

    e4 = new_entity("Line2");
    e4->transform.position = (Vector) {-10, 25};
    e4->transform.rotation = M_PI_4;
    body = new_physics_body((Vector) {0, 0}, 1, Material_CONCRETE, true);
    set_to_polygon_collider(body, (Vector[2]) {
            {0, 0},
            {128, 0}
    }, 2);
    add_physics_body(e4,body);
    add_to_scene(s, e4);

    e4 = new_entity("Line3");
    e4->transform.position = (Vector) {100, 25};
    e4->transform.rotation = 3*M_PI_4;
    body = new_physics_body((Vector) {0, 0}, 1, Material_CONCRETE, true);
    set_to_polygon_collider(body, (Vector[2]) {
            {0, 0},
            {128, 0}
    }, 2);
    add_physics_body(e4,body);
    add_to_scene(s, e4);


//    e3->sprite = load_sprite(&I_big_ball);
//    FURI_LOG_I("AS", "SIZE: %i, %i", icon_get_width(&I_ball), icon_get_height(&I_ball));

//    add_component(e3, init_ball, rotate, sizeof(ball_data));
//    add_component(e3, init_ball, scale, sizeof(ball_data));
//    add_component(e3, init_ball, rotate, sizeof(ball_data));

    //Add to scene

//    add_to_entity(e3, e);
//    add_to_entity(e, e2);
    set_scene(s);
}

void init(void *state) {
    UNUSED(state);
}

int32_t jumper_app(void *p) {
    UNUSED(p);
    int32_t return_code;
    return_code = setup_engine((SetupState) {
            .app_name="Jumper",
            .state_size=sizeof(GameState),  //size of game state
            .init_state=init,               //callback to initialize game state
            .always_on_screen=true,               //keep backlight on
            .tick_rate=30                  //update freq
    });

    if (return_code == 0) {
        setup_play_scene();
        start_loop();           //start main loop
    }

    return return_code;
}