#include "include/input.h"

internal void
initialize_controller(game_input_t *input) 
{
    // Code copied straight from the documentation :)
    for (int i = 0; i < SDL_NumJoysticks(); ++i)
    {
        if (SDL_IsGameController(i))
        {
            input->game_controller = SDL_GameControllerOpen(i);
            if (input->game_controller)
            {
                input->gamepad.controller_connected = true;
                break;
            }
            else
            {
                input->gamepad.controller_connected = false;
                fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
            }
        }
    }
}

internal void
controller_setup(game_input_t *input)
{
    if (SDL_GameControllerGetAttached(input->game_controller))
    {
        input->gamepad.up = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
        input->gamepad.down = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        input->gamepad.left = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        input->gamepad.right = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        input->gamepad.start = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_START);
        input->gamepad.back = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_BACK);
        input->gamepad.left_shoulder = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        input->gamepad.right_shoulder = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        input->gamepad.a_button = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_A);
        input->gamepad.b_button = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_B);
        input->gamepad.x_button = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_X);
        input->gamepad.y_button = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_BUTTON_Y);
        input->gamepad.stick_x = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_AXIS_LEFTX);
        input->gamepad.stick_y = SDL_GameControllerGetButton(input->game_controller, SDL_CONTROLLER_AXIS_LEFTY);
    }
    else
    {
        printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
    }
}

internal bool 
mouse_input(game_t *game, SDL_MouseButtonEvent *mouse_event)
{
    if (mouse_event->button == SDL_BUTTON_LEFT)
    {
        // ex fire or throw something
        
        // If the mouse is down it is pressed
        if (mouse_event->type == SDL_MOUSEBUTTONDOWN)
        {
            printf("Pressed\n");
            return true;
        }
        
        // If mouse is up do not do anything thus not pressed
        if (mouse_event->type == SDL_MOUSEBUTTONUP)
            return false;
    }
    
    return false;
}

internal void 
poll_input(game_t *game, game_input_t *input, SDL_MouseButtonEvent m_event)
{
    // If there is a controller connected set it up
    if (input->gamepad.controller_connected)
        controller_setup(input);
    
    /* Keyboard Code */
    
    const u8 *state = SDL_GetKeyboardState(NULL);
    
    bool pressed = false;
    
    /* Player Movement Code */
    if (game->game_state.playing)
    {
        if (state[SDL_SCANCODE_W] | input->gamepad.up)
        {
            game->player.y--;
            pressed = true;
            
            game->player.direction_index = PLAYER_UP;
        }
        
        if (state[SDL_SCANCODE_A] | input->gamepad.left)
        {
            game->player.x--;
            pressed = true;
            
            game->player.direction_index = PLAYER_LEFT;
        }
        
        if (state[SDL_SCANCODE_S] | input->gamepad.down)
        {
            game->player.y++;
            pressed = true;
            
            game->player.direction_index = PLAYER_DOWN;
        }
        
        if (state[SDL_SCANCODE_D] | input->gamepad.right)
        {
            game->player.x++;
            pressed = true;
            
            game->player.direction_index = PLAYER_RIGHT;
        }
        
        /* Funnel Player Ability Code */
        if (mouse_input(game, &m_event) | input->gamepad.a_button)
            game->player.is_shooting = true;
        else
            game->player.is_shooting = false;
        
        // Controller Buttons
        if (input->gamepad.a_button) {
            printf("A button pressed.\n");
        }
    }
    
    if (pressed)
        game->player.moving = true;
    else
        game->player.moving = false;
    
    /* Start Screen Input */
    if (state[SDL_SCANCODE_F] | input->gamepad.start)
    {
        game->game_state.start_screen = false;
        game->game_state.playing = true;
    }
    
    pressed = !pressed;
}