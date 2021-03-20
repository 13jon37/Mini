#include "include/start_screen.h"

internal bool
load_start_screen_font(start_screen_t *s, buffer_t *buffer)
{
    s->start_screen_font = TTF_OpenFont("Assets/liberation-mono.ttf", 32);
    
    if (!s->start_screen_font)
    {
        printf("Failed to load start screen font.\n");
        return false;
    }
    
    SDL_Color White = { 255, 255, 255, 255 };
    
    const char text_buf[] = "Press F or the Start Button to Play"; 
    
    SDL_Surface *surface = TTF_RenderText_Solid(s->start_screen_font, text_buf, White);
    s->font_texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    return true;
}

internal void 
render_start_screen_text(start_screen_t *s, buffer_t *buffer)
{
    SDL_Rect rect = { GAME_WIDTH / 2 - 64, GAME_HEIGHT / 2 - 16, 128, 32 };
    SDL_RenderCopy(buffer->renderer, s->font_texture, NULL, &rect);
}

internal void 
render_start_screen(start_screen_t *s, buffer_t *buffer)
{
    SDL_RenderClear(buffer->renderer);
    
    // Insert Objects to Render
    SDL_SetRenderDrawColor(buffer->renderer, 0, 0, 0, 255);
    
    render_start_screen_text(s, buffer);
    
    SDL_RenderPresent(buffer->renderer);
}

internal void
destroy_start_screen_font_texture(start_screen_t *s)
{
    SDL_DestroyTexture(s->font_texture);
}