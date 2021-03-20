/* date = March 20th 2021 5:14 pm */

#ifndef START_SCREEN_H
#define START_SCREEN_H

// Sorta OOP for some reason

typedef struct START_SCREEN_STRUCT {
    TTF_Font *start_screen_font;
    SDL_Texture *font_texture;
} start_screen_t;

internal bool load_start_screen_font(start_screen_t *s, buffer_t *buffer);
internal void render_start_screen_text(start_screen_t *s, buffer_t *buffer);
internal void render_start_screen(start_screen_t *s, buffer_t *buffer);

#endif //START_SCREEN_H
