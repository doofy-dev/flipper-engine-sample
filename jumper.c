#include <furi.h>
#include "flipper-game-engine/game_engine.h"
#include "defines.h"
#include "jumper_icons.h"
#include "flipper-game-engine/util/util.h"


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

void scale(ComponentInfo *component, void *state) {
    UNUSED(state);
    add_scale(&(component->entity->transform), (Vector) {0.01, 0.01});
    if (component->entity->transform.scale.x > 4)
        set_scale(&(component->entity->transform), (Vector) {0.1, 0.1});
}

void setup_play_scene() {
    //Create new scene
    Scene *s = new_scene("Play");
    //Create new entity and set up data for it
    entity_t *e = new_entity("Ball");
    e->transform.position = (Vector) {20, -10};
    //Store image that will be drawn on render
    e->sprite = load_sprite(&I_small_ball);
    //Size of the image
    e->sprite.anchor = (Vector) {0.5, 0.5};
    //Enable drawing
    e->draw = true;

    entity_t *e2 = new_entity("Ball2");
    e2->transform.position = (Vector) {-20, -10};
    e2->sprite = load_sprite(&I_big_ball);
    e2->sprite.anchor = (Vector) {0.5, 0.5};
    e2->draw = true;

    entity_t *e3 = new_entity("Ball3");
    e3->transform.position = (Vector) {64, 32};
//    e3->transform.rotation=0.7853982;
    e3->sprite = load_sprite(&I_platforms);
//    e3->sprite = load_sprite(&I_big_ball);
//    FURI_LOG_I("AS", "SIZE: %i, %i", icon_get_width(&I_ball), icon_get_height(&I_ball));
    e3->sprite.anchor = (Vector) {0.5, 0.5};
    e3->draw = true;

    add_component(e, init_ball, rotate, sizeof(ball_data));
//    add_component(e3, init_ball, scale, sizeof(ball_data));
    add_component(e3, init_ball, rotate, sizeof(ball_data));

    //Add to scene

//    add_to_entity(e3, e);
//    add_to_entity(e, e2);
    add_to_scene(s, e3);
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