#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/language_layer.h"

#define GAME_TITLE "Game Title"
#define GAME_WIDTH 512
#define GAME_HEIGHT 288

typedef struct BUFFER_STRUCT {
    SDL_Window* window;
    SDL_Renderer* renderer;
} buffer_t;

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
} controller_t;

typedef struct PLAYER_STRUCT {
    u32 x, y;
    u32 health;
    f32 speed;
    SDL_Texture* texture;
} player_t;

global_variable buffer_t global_buffer;
global_variable player_t global_player;
global_variable u64 global_total_rendered_frames;
global_variable bool global_is_fullscreen;

global_variable SDL_GameController* global_game_controller;

internal void
initialize_controller(void) 
{
    // Code copied straight from the documentation :)
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            global_game_controller = SDL_GameControllerOpen(i);
            if (global_game_controller) {
                break;
            } else {
                fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
            }
        }
    }
}

internal void 
poll_input(void)
{
    controller_t gamepad;
    if(SDL_GameControllerGetAttached(global_game_controller))
    {
        gamepad.up = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
        gamepad.down = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        gamepad.left = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        gamepad.right = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        gamepad.start = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_START);
        gamepad.back = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_BACK);
        gamepad.left_shoulder = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        gamepad.right_shoulder = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        gamepad.a_button = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_A);
        gamepad.b_button = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_B);
        gamepad.x_button = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_X);
        gamepad.y_button = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_Y);
        gamepad.stick_x = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_AXIS_LEFTX);
        gamepad.stick_y = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_AXIS_LEFTY);
    }
    else
    {
        printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
    }
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    if(state[SDL_SCANCODE_W])
    {
        global_player.y--;
    }
    
    if(state[SDL_SCANCODE_A])
    {
        global_player.x--;
    }
    
    if(state[SDL_SCANCODE_S])
    {
        global_player.y++;
    }
    
    if(state[SDL_SCANCODE_D])
    {
        global_player.x++;
    }
    
}

internal bool 
initialize_player(buffer_t* buffer)
{
    global_player.x = 25;
    global_player.y = 25;
    global_player.health = 100;
    
    SDL_Surface* surface = SDL_LoadBMP("Assets/soldier_standing.bmpx");
    global_player.texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!global_player.texture)
    {
        printf("Failed to load player bmp.\n");
        return false;
    }
    
    return true;
}

internal void
player_render(player_t* player, buffer_t* buffer)
{
    SDL_Rect rect = { player->x, player->y, 16, 16 };
    SDL_RenderCopy(buffer->renderer, player->texture, NULL, &rect);
}

internal void
render_buffer_to_screen(buffer_t* buffer)
{
    SDL_RenderClear(buffer->renderer);
    
    // Insert Objects to Render
    SDL_SetRenderDrawColor(buffer->renderer, 0, 0, 0, 255);
    
    player_render(&global_player, buffer);
    
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

internal int
set_fullscreen(void)
{
    SDL_DisplayMode info = get_screen_info();
    
    SDL_SetWindowSize(global_buffer.window, info.w, info.h);
    
    int result = SDL_SetWindowFullscreen(global_buffer.window, SDL_WINDOW_FULLSCREEN);
    
    if (result != 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to set window fullscreen.",
                                 NULL);
    }
    
    return result;
}

internal void
process_events(SDL_Event* event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN: {
            switch (event->key.keysym.sym)
            {
                case SDLK_F11: {
                    
                    if (global_is_fullscreen)
                    {
                        
                        SDL_DisplayMode dm = get_screen_info();
                        SDL_RestoreWindow(global_buffer.window); //Incase it's maximized...
                        SDL_Delay(500);
                        
                        if (SDL_SetWindowFullscreen(global_buffer.window, 0) != 0)
                        {
                            printf("Failed to set windowed\n");
                        }
                        
                        SDL_SetWindowSize(global_buffer.window, dm.w - 80, dm.h - 80);
                        SDL_SetWindowPosition(global_buffer.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
                        global_is_fullscreen = false;
                    }
                    else
                    {
                        set_fullscreen();
                        SDL_Delay(500);
                        global_is_fullscreen = true;
                    }
                    
                } break;
            }
        } break;
    }
    // Poll Keyboard and Controller Input
    poll_input();
}

int main(int argc, char* argv[])
{
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to init SDL.",
                                 NULL);
        goto Exit;
    }
    
    /* Get Screen Size */
    SDL_DisplayMode display_mode_info = get_screen_info();
    
    global_buffer.window = SDL_CreateWindow(GAME_TITLE, 
                                            SDL_WINDOWPOS_UNDEFINED, 
                                            SDL_WINDOWPOS_UNDEFINED,
                                            display_mode_info.w,
                                            display_mode_info.h,
                                            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (!global_buffer.window) 
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to create window.",
                                 NULL);
        goto Exit;
    }
    
    global_buffer.renderer = SDL_CreateRenderer(global_buffer.window, 
                                                -1,  // GPU thingy
                                                SDL_RENDERER_ACCELERATED); // Flags
    
    // Set logical render -- basically set buffer render res
    if (SDL_RenderSetLogicalSize(global_buffer.renderer, GAME_WIDTH, GAME_HEIGHT) != 0)
    {
        printf("Failed to set logical game res.\n");
        goto Exit;
    }
    
    global_is_fullscreen = true;
    
    set_fullscreen();
    
    initialize_controller();
    
    if (!initialize_player(&global_buffer))
    {
        printf("Failed to initalize player!\n");
        goto Exit;
    }
    
    SDL_Event event;
    bool running = true;
    
    i32 target_fps = 60;
    i32 desired_delta= 1000 / target_fps;
    
    /* Main Game Loop */
    while (running)
    {
        i32 start_counter = SDL_GetTicks();
        /* Event Loop */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }
        
        /* Main Game Simulation */
        {
            process_events(&event);
            render_buffer_to_screen(&global_buffer); 
        }
        
        global_total_rendered_frames++;
        i32 delta = SDL_GetTicks() - start_counter;
        if (delta < desired_delta)
        {
            SDL_Delay(desired_delta - delta);
        }
    }
    
    
    Exit:
    SDL_GameControllerClose(global_game_controller);
    SDL_DestroyTexture(global_player.texture);
    SDL_DestroyRenderer(global_buffer.renderer);
    SDL_DestroyWindow(global_buffer.window);
    SDL_Quit();
    
    return 0; 
}
