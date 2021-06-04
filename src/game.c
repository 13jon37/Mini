#include "video.c"
#include "render.c"
#include "start_screen.c"
#include "audio.c"
#include "input.c"

internal void
mutable_objects(game_t *game, buffer_t *buffer)
{
    // Maybe have "mutable" objects in a seperate void function for simple organization
    // since these objects will only have short term existence in comaprison to others
    
    // Idk what the fuck im doing, i'm confused asf
    if (game->entities.player.is_shooting)
        render_bullet(game, buffer);
}

internal void
render_buffer_to_screen(game_t *game, buffer_t *buffer)
{
    SDL_RenderClear(buffer->renderer);
    
    // Insert Objects to Render
    SDL_SetRenderDrawColor(buffer->renderer, 0, 0, 0, 255);
    
    render_tiles(game, buffer);
    
    player_render(&game->entities.player, buffer);
    
    enemy_render(&game->entities.enemy, buffer);
    
    mutable_objects(game, buffer);
    
    render_cursor(game, buffer);
    
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
    
    if (!load_tiles(game, buffer))
        return false;
    
    if (!load_bullet(game, buffer))
        return false;
    
    if (!initialize_player(&game->entities.player, buffer))
    {
        printf("Failed to initialize player!\n");
        return false;
    }
    
    game->entities.enemy = *initialize_enemy(buffer);
    
    if (&game->entities.enemy == (enemy_t*)0)
    {
        printf("Failed to initialize enemy!\n");
        return false;
    }
    
    return true;
}