/* date = January 8th 2021 5:07 am */

#ifndef GAME_H
#define GAME_H

#include "player.h"

#define GAME_TITLE   "Mini Gamer"
#define GAME_SIZE    16         /* Game is 16 x 16 */
#define GAME_WIDTH   512
#define GAME_HEIGHT  288

typedef struct BUFFER_STRUCT {
    SDL_Window *window;
    SDL_Renderer *renderer;
} buffer_t;

typedef struct TILE_STRUCT {
    u32 x, y;
    SDL_Texture *texture;
} tile_t;

typedef struct PERFORMANCE_DATA_STRUCT {
    f32 frames_per_second;
    u64 total_rendered_frames;
    bool is_fullscreen;
    TTF_Font *fps_font;
    SDL_Texture *fps_texture;
} performance_t;

typedef struct GAME_STATE_STRUCT {
    bool start_screen;
    bool playing;
} game_state_t;

typedef struct GAME_STRUCT {
    game_state_t game_state;
    tile_t tiles;
    player_t player;
    SDL_Texture *cursor_texture;
    SDL_Texture *bullet_texture;
} game_t;

#endif //GAME_H
