#include "include/start_screen.h"

internal
void render_start_screen(start_screen_t *s, buffer_t *buffer)
{
    SDL_RenderClear(buffer->renderer);
    
    // Insert Objects to Render
    SDL_SetRenderDrawColor(buffer->renderer, 0, 0, 0, 255);
    
    SDL_RenderPresent(buffer->renderer);
}