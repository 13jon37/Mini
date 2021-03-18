/* date = March 17th 2021 10:01 pm */

#ifndef INPUT_H
#define INPUT_H

typedef struct CONTROLLER_STRUCT {
    bool up;
    bool down;
    bool left;
    bool right;
    bool start;
    bool back;
    bool left_shoulder;
    bool right_shoulder;
    bool a_button;
    bool b_button;
    bool x_button;
    bool y_button;
    i16 stick_x;
    i16 stick_y;
    bool controller_connected;
} controller_t;

typedef struct GAME_INPUT_STRUCT {
    SDL_GameController* game_controller;
    controller_t gamepad;
} game_input_t;

internal void initialize_controller(game_input_t* input);
internal void controller_setup(game_input_t* input);
internal bool mouse_input(game_t* game, SDL_MouseButtonEvent* mouse_event);
internal void poll_input(game_t* game, game_input_t* input, SDL_MouseButtonEvent m_event);

#endif //INPUT_H
