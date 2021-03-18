#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "include/language_layer.h"
#include "include/game.h"

#include "render.c"
#include "audio.c"

global_variable game_t global_game;
global_variable performance_t global_performance_data;

internal void
initialize_controller(game_input_t* input) 
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
controller_setup(game_input_t* input)
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
mouse_input(game_t* game, SDL_MouseButtonEvent* mouse_event)
{
    bool pressed = false;
    
    if (mouse_event->button == SDL_BUTTON_LEFT)
    {
        // ex fire or throw something
        
        // If the mouse is down it is pressed
        if (mouse_event->type == SDL_MOUSEBUTTONDOWN)
        {
            pressed = true;
            printf("Pressed\n");
        }
        
        // If mouse is up do not do anything thus not pressed
        if (mouse_event->type == SDL_MOUSEBUTTONUP)
            pressed = false;
    }
    
    return pressed;
}

internal void 
poll_input(game_t* game, game_input_t* input, SDL_MouseButtonEvent m_event)
{
    // If there is a controller connected set it up
    if (input->gamepad.controller_connected)
        controller_setup(input);
    
    /* Keyboard Code */
    
    const u8 *state = SDL_GetKeyboardState(NULL);
    
    bool pressed = false;
    
    /* Player Movement Code */
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
    
    // Controller Buttons
    if (input->gamepad.a_button) {
        printf("A button pressed.\n");
    }
    
    if (pressed)
        game->player.moving = true;
    else
        game->player.moving = false;
    
    /* Funnel Player Ability Code */
    if (mouse_input(game, &m_event))
        game->player.is_shooting = true;
    else
        game->player.is_shooting = false;
    
    pressed = !pressed;
}

internal bool 
initialize_player(game_t* game, buffer_t* buffer)
{
    game->player.x = (GAME_WIDTH / 2) - GAME_SIZE;
    game->player.y = (GAME_HEIGHT / 2) - GAME_SIZE;
    game->player.health = 100;
    game->player.moving = false;
    game->player.direction_index = 0;
    game->player.is_shooting = false;
    
    game->player.player_render_rect.x = 0;
    game->player.player_render_rect.y = 0;
    game->player.player_render_rect.w = GAME_SIZE;
    game->player.player_render_rect.h = GAME_SIZE;
    
    SDL_Surface* surface = IMG_Load("Assets/new_sheet.png");
    game->player.sheet_texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!game->player.sheet_texture)
    {
        printf("Failed to load sprite sheet png.\n");
        return false;
    }
    
    SDL_QueryTexture(game->player.sheet_texture,
                     NULL,
                     NULL,
                     &game->player.text_rect.w,
                     &game->player.text_rect.h);
    
    return true;
}

internal void 
player_sprite_animations(game_t* game)
{
    u32 ticks = SDL_GetTicks();
    u32 sprite = (ticks / 100) % 3;
    
    // If moving set animation in proper direction
    if (game->player.moving && game->player.direction_index == PLAYER_DOWN)
    {
        game->player.player_render_rect.y = 0;
        game->player.player_render_rect.x = sprite << 4;
    }
    
    if (game->player.moving && game->player.direction_index == PLAYER_UP)
    {
        game->player.player_render_rect.y = 48;
        game->player.player_render_rect.x = sprite << 4;
    }
    
    if (game->player.moving && game->player.direction_index == PLAYER_RIGHT)
    {
        game->player.player_render_rect.y = 32;
        game->player.player_render_rect.x = sprite << 4;
    }
    
    if (game->player.moving && game->player.direction_index == PLAYER_LEFT)
    {
        game->player.player_render_rect.y = 16;
        game->player.player_render_rect.x = sprite << 4;
    }
    
    // If not moving set idle to the direction last moving
    if (!game->player.moving && game->player.direction_index == PLAYER_DOWN)
    {
        game->player.player_render_rect.x = 0;
        game->player.player_render_rect.y = 0;
    }
    
    if (!game->player.moving && game->player.direction_index == PLAYER_UP)
    {
        game->player.player_render_rect.x = 0;
        game->player.player_render_rect.y = 48;
    }
    
    if (!game->player.moving && game->player.direction_index == PLAYER_RIGHT)
    {
        game->player.player_render_rect.x = 0;
        game->player.player_render_rect.y = 32;
    }
    
    if (!game->player.moving && game->player.direction_index == PLAYER_LEFT)
    {
        game->player.player_render_rect.x = 0;
        game->player.player_render_rect.y = 16;
    }
}

internal void
player_render(game_t* game, buffer_t* buffer)
{
    SDL_Rect rect = { game->player.x, game->player.y,
        game->player.text_rect.w / 3, game->player.text_rect.h / 4 };
    
    player_sprite_animations(game);
    
    SDL_RenderCopy(buffer->renderer, game->player.sheet_texture, &game->player.player_render_rect, &rect);
}

internal void
render_cursor(game_t* game, buffer_t* buffer)
{
    i32 mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    
    SDL_Rect rect = { mouse_x, mouse_y, 8, 8 };
    
    SDL_RenderCopy(buffer->renderer, game->cursor_texture, NULL, &rect);
}

internal void
render_buffer_to_screen(game_t* game, buffer_t* buffer)
{
    SDL_RenderClear(buffer->renderer);
    
    // Insert Objects to Render
    SDL_SetRenderDrawColor(buffer->renderer, 0, 0, 0, 255);
    
    render_tiles(game, buffer);
    
    player_render(game, buffer);
    
    render_cursor(game, buffer);
    
    if (game->player.is_shooting)
        render_bullet(game, buffer);
    
    render_fps_text(&global_performance_data, buffer, global_performance_data.frames_per_second);
    
    SDL_RenderPresent(buffer->renderer);
}

internal SDL_DisplayMode 
get_screen_info(void)
{
    SDL_DisplayMode result = { 0 };
    
    if (SDL_GetDesktopDisplayMode(0, &result) != 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to get displaymode.",
                                 NULL);
    }
    
    return result;
}

internal void
set_fullscreen(buffer_t* buffer)
{
    SDL_DisplayMode info = get_screen_info();
    
    SDL_SetWindowSize(buffer->window, info.w, info.h);
    
    if (SDL_SetWindowFullscreen(buffer->window, SDL_WINDOW_FULLSCREEN) != 0) {
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to set window fullscreen.",
                                 NULL);
    }
}

internal void
set_windowed(buffer_t* buffer) 
{
    SDL_DisplayMode info = get_screen_info();
    
    if (SDL_SetWindowFullscreen(buffer->window, 0) != 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to set screen to windowed mode.",
                                 NULL);
    }
    
    SDL_SetWindowSize(buffer->window, info.w - 80, info.h - 80);
    SDL_SetWindowPosition(buffer->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

internal void
process_events(game_t* game, game_input_t* input, buffer_t* buffer, SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN: {
            switch (event->key.keysym.sym)
            {
                case SDLK_F11: {
                    if (global_performance_data.is_fullscreen)
                        set_windowed(buffer);
                    else
                        set_fullscreen(buffer);
                    global_performance_data.is_fullscreen = !global_performance_data.is_fullscreen;
                } break;
            }
        } break;
    }
    // Poll Keyboard, Mouse, and Controller Input
    poll_input(game, input, event->button);
}

internal bool
initialize_game(game_t* game, buffer_t* buffer, audio_t* audio)
{
    // Set logical render -- basically set buffer render res
    if (SDL_RenderSetLogicalSize(buffer->renderer, GAME_WIDTH, GAME_HEIGHT) != 0)
    {
        printf("Failed to set logical game res.\n");
        return false;
    }
    
    if (global_performance_data.is_fullscreen)
        set_fullscreen(buffer);
    else
        set_windowed(buffer);
    
    // Code to load custom cursor
    
    /*SDL_ShowCursor(0);
    SDL_Surface* surface = IMG_Load("Assets/cursor.png");
    global_game.cursor_texture = SDL_CreateTextureFromSurface(global_buffer.renderer, surface);
    SDL_FreeSurface(surface);
    if (!global_game.cursor_texture)
        return false;*/
    
    if (!load_audio(audio))
        return false;
    
    if (!load_fonts(&global_performance_data))
        return false;
    
    if (!load_tiles(buffer, game))
        return false;
    
    if (!load_bullet(buffer, game))
        return false;
    
    if (!initialize_player(game, buffer))
    {
        printf("Failed to initialize player!\n");
        return false;
    }
    
    return true;
}

int main(int argc, char* argv[])
{
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to init SDL.",
                                 NULL);
        goto Exit;
    }
    
    buffer_t buffer = { 0 };
    
    /* Get Screen Size */
    SDL_DisplayMode display_mode_info = get_screen_info();
    
    buffer.window = SDL_CreateWindow(GAME_TITLE, 
                                     SDL_WINDOWPOS_UNDEFINED, 
                                     SDL_WINDOWPOS_UNDEFINED,
                                     display_mode_info.w,
                                     display_mode_info.h,
                                     SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (!buffer.window) 
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to create window.",
                                 NULL);
        goto Exit;
    }
    
    buffer.renderer = SDL_CreateRenderer(buffer.window, 
                                         -1,  // GPU thingy
                                         SDL_RENDERER_ACCELERATED); // Flags
    game_t game = { 0 };
    audio_t audio = { 0 };
    game_input_t input = { 0 };
    
    if (!initialize_game(&game, &buffer, &audio))
        goto Exit;
    
    SDL_Event event;
    bool running = true;
    
    f32 target_fps = 60.0f;
    f32 desired_delta = 1000.0 / target_fps;
    
    /* Main Game Loop */
    while (running)
    {
        f32 start_counter = SDL_GetTicks();
        /* Event Loop */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }
        
        /* Always check for controller so, 
you dont have to restart the game 
in order to use a controller */
        
        initialize_controller(&input);
        
        /* Main Game Simulation */
        {
            process_events(&game, &input, &buffer, &event);
            render_buffer_to_screen(&game, &buffer); 
        }
        
        global_performance_data.total_rendered_frames++;
        f32 delta = SDL_GetTicks() - start_counter;
        if (delta < desired_delta)
        {
            SDL_Delay(desired_delta - delta);
        }
        
        // Convert m/s to fps
        global_performance_data.frames_per_second = 1000.0 / (f32)(desired_delta - delta);
    }
    
    
    Exit:
    SDL_GameControllerClose(input.game_controller);
    SDL_DestroyTexture(global_game.tiles.texture);
    SDL_DestroyTexture(global_game.player.sheet_texture);
    SDL_DestroyTexture(global_game.cursor_texture);
    SDL_DestroyRenderer(buffer.renderer);
    SDL_DestroyWindow(buffer.window);
    
    free_audio(&audio);
    
    TTF_CloseFont(global_performance_data.fps_font);
    TTF_Quit();
    
    SDL_Quit();
    
    return 0; 
}

