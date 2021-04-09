
/* date = March 20th 2021 5:53 pm */

#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_DOWN  0
#define PLAYER_UP    1
#define PLAYER_RIGHT 2
#define PLAYER_LEFT  3

typedef struct PLAYER_STRUCT {
    u32 x, y;
    u32 health;
    f32 speed;
    bool moving;
    u8 direction_index;
    SDL_Texture *sheet_texture;
    SDL_Rect text_rect;
    SDL_Rect player_render_rect;
    bool is_shooting;
} player_t;

#endif //PLAYER_H
