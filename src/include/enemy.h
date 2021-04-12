#ifndef ENEMY_H
#define ENEMY_H

//
//    Temporary enemy entity for experimentation
//
//    Will probably need dynamic list for better implementation

typedef struct ENEMY_STRUCT {
    u32 x, y;
    u32 health;
    f32 speed;
    SDL_Texture *texture;
    SDL_Rect text_rect;
    SDL_Rect enemy_render_rect;
} enemy_t;

internal enemy_t *initialize_enemy(buffer_t *buffer);
internal void render_enemy(buffer_t *buffer, enemy_t *enemy);
internal void destroy_enemy(enemy_t *enemy);

#endif // ENEMY_H
