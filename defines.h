#pragma once
#include <furi.h>
#include <input/input.h>
#include <gui/elements.h>
#include <flipper_format/flipper_format.h>
#include <flipper_format/flipper_format_i.h>

#define APP_NAME "Jumper"

typedef enum {
    GameStateGameOver,
    GameStateStart,
    GameStatePlay,
    GameStateAnimate
} PlayState;

typedef struct {
    PlayState state;
} GameState;