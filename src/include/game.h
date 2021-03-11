/* date = January 8th 2021 5:07 am */

#ifndef GAME_H
#define GAME_H

#define GAME_TITLE   "Mini Gamer"
#define GAME_SIZE    16         /* Game is 16 x 16 */
#define GAME_WIDTH   512
#define GAME_HEIGHT  288

#define PLAYER_DOWN  0
#define PLAYER_UP    1
#define PLAYER_RIGHT 2
#define PLAYER_LEFT  3

/* Trying to implement ECS */

// https://gamedev.stackexchange.com/questions/172584/how-could-i-implement-an-ecs-in-c

typedef struct POSITION_STRUCT {
    u32 entity_id;
    u32 x;
    u32 y;
} position_t;

typedef struct COMPONENT_LIST_STRUCT {
    position_t position_componenets[100]; // I have no idea
    u32 total_position_components;
} component_lists_t;

/* ************************************ */

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
    u8 direction_index;
    SDL_Texture* sheet_texture;
    SDL_Rect text_rect;
    SDL_Rect player_render_rect;
    bool is_shooting;
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
    SDL_Texture* cursor_texture;
    SDL_AudioSpec wav_spec;
    u32 wav_length;
    u8 *wav_buffer;
    SDL_AudioDeviceID device_id;
    SDL_Texture* bullet_texture;
} game_t;

#endif //GAME_H
