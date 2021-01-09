/* date = January 8th 2021 5:07 am */

#ifndef GAME_H
#define GAME_H

#define GAME_TITLE "Pickaxe Man"
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
    SDL_Texture* sheet_texture;
    SDL_Rect text_rect;
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

#endif //GAME_H
