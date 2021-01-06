#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
    bool controller_connected;
} controller_t;

typedef struct PLAYER_STRUCT {
    u32 x, y;
    u32 health;
    f32 speed;
    bool moving;
    SDL_Texture* texture;
} player_t;

typedef struct TILE_STRUCT {
    u32 x, y;
    SDL_Texture* texture;
} tile_t;

typedef struct PERFORMANCE_DATA_STRUCT {
    f32 frames_per_second;
    u64 total_rendered_frames;
    bool is_fullscreen;
    TTF_Font* fps_font;
    SDL_Texture* fps_texture;
} performance_t;

typedef struct GAME_STRUCT {
    tile_t tiles;
    player_t player;
    controller_t gamepad;
} game_t;

global_variable buffer_t global_buffer;
global_variable game_t global_game;
global_variable performance_t global_performance_data;

global_variable SDL_GameController* global_game_controller;

internal void
initialize_controller(game_t* game) 
{
    // Code copied straight from the documentation :)
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            global_game_controller = SDL_GameControllerOpen(i);
            if (global_game_controller) {
                game->gamepad.controller_connected = true;
                break;
            } else {
                fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
            }
        }
    }
}

internal void
controller_setup(game_t* game)
{
    if (SDL_GameControllerGetAttached(global_game_controller))
    {
        game->gamepad.up = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
        game->gamepad.down = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
        game->gamepad.left = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        game->gamepad.right = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        game->gamepad.start = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_START);
        game->gamepad.back = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_BACK);
        game->gamepad.left_shoulder = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
        game->gamepad.right_shoulder = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
        game->gamepad.a_button = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_A);
        game->gamepad.b_button = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_B);
        game->gamepad.x_button = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_X);
        game->gamepad.y_button = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_BUTTON_Y);
        game->gamepad.stick_x = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_AXIS_LEFTX);
        game->gamepad.stick_y = SDL_GameControllerGetButton(global_game_controller, SDL_CONTROLLER_AXIS_LEFTY);
    }
    else
    {
        printf("Warning: Unable to open game controller or no controller plugged in! SDL Error: %s\n", SDL_GetError());
    }
}

internal void 
poll_input(game_t* game)
{
    // If there is a controller connected set it up
    if (game->gamepad.controller_connected)
        controller_setup(game);
    
    /* Player Movement Code */
    
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    
    if (state[SDL_SCANCODE_W] | game->gamepad.up)
    {
        global_game.player.y--;
    }
    
    if (state[SDL_SCANCODE_A] |  game->gamepad.left)
    {
        global_game.player.x--;
    }
    
    if (state[SDL_SCANCODE_S] | game->gamepad.down)
    {
        global_game.player.y++;
    }
    
    if (state[SDL_SCANCODE_D] | game->gamepad.right)
    {
        global_game.player.x++;
    }
}

internal bool 
load_tiles(buffer_t* buffer, game_t* game)
{
    SDL_Surface* surface = IMG_Load("Assets/grass.png");
    game->tiles.texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!game->tiles.texture)
    {
        printf("Failed to load tile texture.\n");
        return false;
    }
    
    return true;
}

internal void
render_tiles(game_t* game, buffer_t* buffer)
{
    for (i32 y = 0; y < GAME_HEIGHT; y+=16)
    {
        for (i32 x = 0; x < GAME_WIDTH; x+=16)
        {
            SDL_Rect rect = { x, y, 16, 16 };
            SDL_RenderCopy(buffer->renderer, game->tiles.texture, NULL, &rect);
        }
    }
}

internal bool
load_fonts(performance_t* perf)
{
    if (TTF_Init() == -1) {
        printf("Failed to init TTF.\n");
        return false;
    }
    
    perf->fps_font = TTF_OpenFont("Assets/liberation-mono.ttf", 12);
    
    if (!perf->fps_font)
    {
        printf("Failed to load fonts.\n");
        return false;
    }
    
    return true;
}

internal void
render_fps_text(performance_t* perf, buffer_t* buffer, f32 fps)
{
    SDL_Color Red = { 255, 0, 0, 0 };
    
    char fps_buf[12];
    
    gcvt(fps, 2, fps_buf);
    
    SDL_Surface* surface = TTF_RenderText_Solid(perf->fps_font, fps_buf, Red);
    perf->fps_texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    SDL_Rect rect = { 5, 0, 10, 10};
    SDL_RenderCopy(buffer->renderer, perf->fps_texture, NULL, &rect);
    
    SDL_DestroyTexture(perf->fps_texture);
}


internal bool 
initialize_player(buffer_t* buffer)
{
    global_game.player.x = 25;
    global_game.player.y = 25;
    global_game.player.health = 100;
    
    SDL_Surface* surface = SDL_LoadBMP("Assets/soldier_standing.bmpx");
    global_game.player.texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!global_game.player.texture)
    {
        printf("Failed to load player bmp.\n");
        return false;
    }
    
    return true;
}

internal void
player_render(game_t* game, buffer_t* buffer)
{
    SDL_Rect rect = { game->player.x, game->player.y, 16, 16 };
    SDL_RenderCopy(buffer->renderer, game->player.texture, NULL, &rect);
}

internal void
render_buffer_to_screen(game_t* game, buffer_t* buffer)
{
    SDL_RenderClear(buffer->renderer);
    
    // Insert Objects to Render
    SDL_SetRenderDrawColor(buffer->renderer, 0, 0, 0, 255);
    
    render_tiles(game, buffer);
    
    player_render(game, buffer);
    
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
set_fullscreen(void)
{
    SDL_DisplayMode info = get_screen_info();
    
    SDL_SetWindowSize(global_buffer.window, info.w, info.h);
    
    if (SDL_SetWindowFullscreen(global_buffer.window, SDL_WINDOW_FULLSCREEN) != 0) {
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to set window fullscreen.",
                                 NULL);
    }
}

internal void
set_windowed(void) 
{
    SDL_DisplayMode info = get_screen_info();
    
    if (SDL_SetWindowFullscreen(global_buffer.window, 0) != 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to set screen to windowed mode.",
                                 NULL);
    }
    
    SDL_SetWindowSize(global_buffer.window, info.w - 80, info.h - 80);
    SDL_SetWindowPosition(global_buffer.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
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
                    
                    if (global_performance_data.is_fullscreen)
                        set_windowed();
                    else
                        set_fullscreen();
                    global_performance_data.is_fullscreen = !global_performance_data.is_fullscreen;
                } break;
            }
        } break;
    }
    // Poll Keyboard and Controller Input
    poll_input(&global_game);
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
    
    if (global_performance_data.is_fullscreen)
        set_fullscreen();
    else
        set_windowed();
    
    if (!load_tiles(&global_buffer, &global_game))
        goto Exit;
    
    if (!load_fonts(&global_performance_data))
        goto Exit;
    
    if (!initialize_player(&global_buffer))
    {
        printf("Failed to initalize player!\n");
        goto Exit;
    }
    
    SDL_Event event;
    bool running = true;
    
    f32 target_fps = 60.0f;
    f32 desired_delta= 1000.0 / target_fps;
    
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
        
        /* Always check for controller so, 
you dont have to restart the game 
in order to use a controller */
        
        initialize_controller(&global_game);
        
        /* Main Game Simulation */
        {
            process_events(&event);
            render_buffer_to_screen(&global_game, &global_buffer); 
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
    SDL_GameControllerClose(global_game_controller);
    SDL_DestroyTexture(global_game.tiles.texture);
    SDL_DestroyTexture(global_game.player.texture);
    SDL_DestroyRenderer(global_buffer.renderer);
    SDL_DestroyWindow(global_buffer.window);
    
    TTF_CloseFont(global_performance_data.fps_font);
    TTF_Quit();
    
    SDL_Quit();
    
    return 0; 
}
