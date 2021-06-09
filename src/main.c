#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "include/language_layer.h"
#include "include/game.h"

#include "enemy.c"
#include "player.c"

global_variable performance_t global_performance_data;

#include "game.c"

/* *************************
Ideas to further this project:

- Implement a tile map
- Increase focus on gameplay functions rather than engine functions
- Faction Selection Scene
- 

 *************************** */

int main(int argc, char *argv[])
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
    
    buffer.renderer = SDL_CreateRenderer(buffer.window,             // Window
                                         -1,                        // GPU thingy
                                         SDL_RENDERER_ACCELERATED); // Flags
    
    game_t game = { 0 };
    
    // NOTE(Jon): messy asf and temporary most likely
    
    // Check for monitor width so if the res is over 1920 we can scale 
    // the game up in order for it to look better
    if (display_mode_info.w == 1920)
    {
        //game.render_res_w = 640; // test values
        //game.render_res_h = 360;
        game.render_res_w = 384; // test values
        game.render_res_h = 216;
    }
    else
    {
        game.render_res_w = GAME_WIDTH;
        game.render_res_h = GAME_HEIGHT;
    }
    
    // Set logical render -- basically set buffer render res
    if (SDL_RenderSetLogicalSize(buffer.renderer, game.render_res_w, game.render_res_h) != 0)
    {
        printf("Failed to set logical game res.\n");
        goto Exit;
    }
    
    global_performance_data.is_fullscreen = true;
    
    if (global_performance_data.is_fullscreen)
        set_fullscreen(&buffer);
    else
        set_windowed(&buffer);
    
    start_screen_t start_screen = { 0 };
    audio_t audio = { 0 };
    game_input_t input = { 0 };
    
    if (!initialize_game(&game, &start_screen, &buffer, &audio))
        goto Exit;
    
    game.game_state.start_screen = true;
    
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
        
        initialize_controller(&input); // Always checks for a controller connection
        
        /* Main Game Simulation */
        {
            process_events(&game, &input, &buffer, &event);
            
            if (game.game_state.start_screen)
                render_start_screen(&start_screen, &game, &buffer);
            else if (game.game_state.playing)
            {
                // Autistic asf but I don't want textures loaded I don't need
                if (start_screen.font_texture)
                    destroy_start_screen_font_texture(&start_screen);
                
                render_buffer_to_screen(&game, &buffer);
            }
        }
        
        if (global_performance_data.total_rendered_frames == 250)
            destroy_enemy(&game.entities.enemy);
        
        global_performance_data.total_rendered_frames++;
        f32 delta = SDL_GetTicks() - start_counter;
        if (delta < desired_delta)
            SDL_Delay(desired_delta - delta);
        
        // Convert m/s to fps
        global_performance_data.frames_per_second = 1000.0 / (f32)(desired_delta - delta);
    }
    
    Exit:
    SDL_GameControllerClose(input.game_controller);
    
    SDL_DestroyTexture(game.tiles.texture);
    SDL_DestroyTexture(game.entities.player.sheet_texture);
    SDL_DestroyTexture(game.cursor_texture);
    
    destroy_enemy(&game.entities.enemy);
    free_audio(&audio);
    
    SDL_DestroyRenderer(buffer.renderer);
    SDL_DestroyWindow(buffer.window);
    
    TTF_CloseFont(start_screen.start_screen_font);
    TTF_CloseFont(global_performance_data.fps_font);
    TTF_Quit();
    
    SDL_Quit();
    
    return 0;
}
