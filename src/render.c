internal bool
load_fonts(performance_t *perf)
{
    if (TTF_Init() == -1) {
        printf("Failed to init TTF.\n");
        return false;
    }
    
    perf->fps_font = TTF_OpenFont("Assets/liberation-mono.ttf", 12);
    
    if (!perf->fps_font)
    {
        printf("Failed to load fonts.\n");
        return false;
    }
    
    return true;
}

internal void
render_fps_text(performance_t *perf, buffer_t *buffer, f32 fps)
{
    SDL_Color Red = { 255, 0, 0, 0 };
    
    char fps_buf[12];
    
    // char *gcvt(double x, int ndigit, char *buf);
    // Converts floating point number to string
    gcvt(fps, 2, fps_buf);
    
    SDL_Surface* surface = TTF_RenderText_Solid(perf->fps_font, fps_buf, Red);
    perf->fps_texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    SDL_Rect rect = { 5, 0, 10, 10};
    SDL_RenderCopy(buffer->renderer, perf->fps_texture, NULL, &rect);
    
    SDL_DestroyTexture(perf->fps_texture);
}

internal bool 
load_tiles(buffer_t *buffer, game_t *game)
{
    SDL_Surface* surface = IMG_Load("Assets/grass.png");
    game->tiles.texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!game->tiles.texture)
    {
        printf("Failed to load tile texture.\n");
        return false;
    }
    
    return true;
}

internal void
render_tiles(game_t *game, buffer_t *buffer)
{
    for (u32 y = 0; y < GAME_HEIGHT; y+=GAME_SIZE)
    {
        for (u32 x = 0; x < GAME_WIDTH; x+=GAME_SIZE)
        {
            SDL_Rect rect = { x, y, GAME_SIZE, GAME_SIZE };
            SDL_RenderCopy(buffer->renderer, game->tiles.texture, NULL, &rect);
        }
    }
}

internal bool
load_bullet(buffer_t *buffer, game_t *game)
{
    SDL_Surface* surface = IMG_Load("Assets/bullet.png");
    game->bullet_texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!game->bullet_texture)
    {
        printf("Failed to load bullet texture.\n");
        return false;
    }
    
    return true;
}

internal void
render_bullet(game_t *game, buffer_t *buffer)
{
    SDL_Rect rect = { 0, 0, GAME_SIZE, GAME_SIZE };
    SDL_RenderCopy(buffer->renderer, game->bullet_texture, NULL, &rect);
}

internal void
render_cursor(game_t *game, buffer_t *buffer)
{
    i32 mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    
    SDL_Rect rect = { mouse_x, mouse_y, 8, 8 };
    
    SDL_RenderCopy(buffer->renderer, game->cursor_texture, NULL, &rect);
}


