#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "include/language_layer.h"
#include "include/game.h"

#include "video.c"
#include "render.c"
#include "start_screen.c"
#include "audio.c"
#include "input.c"

#include "player.c"
#include "enemy.c"

global_variable performance_t global_performance_data;
global_variable enemy_t *global_enemy;

internal void
render_buffer_to_screen(game_t *game, buffer_t *buffer)
{
    SDL_RenderClear(buffer->renderer);

    // Insert Objects to Render
    SDL_SetRenderDrawColor(buffer->renderer, 0, 0, 0, 255);

    render_tiles(game, buffer);

    player_render(game, buffer);

    render_enemy(buffer, &*global_enemy);

    render_cursor(game, buffer);

    if (game->player.is_shooting)
        render_bullet(game, buffer);

    render_fps_text(&global_performance_data, buffer, global_performance_data.frames_per_second);

    SDL_RenderPresent(buffer->renderer);
}

internal void
process_events(game_t *game,
               game_input_t *input,
               buffer_t *buffer,
               SDL_Event *event)
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
initialize_game(game_t *game,
                start_screen_t *start_screen,
                buffer_t *buffer,
                audio_t *audio)
{
    // Set logical render -- basically set buffer render res
    if (SDL_RenderSetLogicalSize(buffer->renderer, GAME_WIDTH, GAME_HEIGHT) != 0)
    {
        printf("Failed to set logical game res.\n");
        return false;
    }

    global_performance_data.is_fullscreen = true;

    if (global_performance_data.is_fullscreen)
        set_fullscreen(buffer);
    else
        set_windowed(buffer);

    // Code to load custom cursor

    /*
SDL_ShowCursor(0);
    SDL_Surface* surface = IMG_Load("Assets/cursor.png");
    global_game.cursor_texture = SDL_CreateTextureFromSurface(global_buffer.renderer, surface);
    SDL_FreeSurface(surface);
    if (!global_game.cursor_texture)
        return false;
*/

    if (!load_audio(audio))
        return false;

    if (!load_fonts(&global_performance_data))
        return false;

    if (!load_start_screen_font(start_screen, buffer))
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

    if ((global_enemy = initialize_enemy(buffer)) == (enemy_t*)0)
    {
        printf("Failed to initialize enemy!\n");
        return false;
    }

    return true;
}

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

    buffer.renderer = SDL_CreateRenderer(buffer.window,
                                         -1,                        // GPU thingy
                                         SDL_RENDERER_ACCELERATED); // Flags
    game_t game = { 0 };
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
            {
                render_start_screen(&start_screen, &buffer);
            }
            else if (game.game_state.playing)
            {
                // Autistic asf but I don't want textures loaded I don't need
                if (start_screen.font_texture)
                    destroy_start_screen_font_texture(&start_screen);

                render_buffer_to_screen(&game, &buffer);
            }
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
    SDL_DestroyTexture(game.tiles.texture);
    SDL_DestroyTexture(game.player.sheet_texture);
    SDL_DestroyTexture(game.cursor_texture);
    SDL_DestroyRenderer(buffer.renderer);
    SDL_DestroyWindow(buffer.window);

    free_audio(&audio);
    free(global_enemy);

    TTF_CloseFont(start_screen.start_screen_font);
    TTF_CloseFont(global_performance_data.fps_font);
    TTF_Quit();

    SDL_Quit();

    return 0;
}
