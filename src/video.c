internal SDL_DisplayMode 
get_screen_info(void)
{
    SDL_DisplayMode result = { 0 };
    
    if (SDL_GetDesktopDisplayMode(0, &result) != 0)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to get displaymode.",
                                 NULL);
    }
    
    return result;
}

internal void
set_fullscreen(buffer_t *buffer)
{
    SDL_DisplayMode info = get_screen_info();
    
    SDL_SetWindowSize(buffer->window, info.w, info.h);
    
    if (SDL_SetWindowFullscreen(buffer->window, SDL_WINDOW_FULLSCREEN) != 0) {
        
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to set window fullscreen.",
                                 NULL);
    }
}

internal void
set_windowed(buffer_t *buffer) 
{
    SDL_DisplayMode info = get_screen_info();
    
    if (SDL_SetWindowFullscreen(buffer->window, 0) != 0) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                                 "Error",
                                 "Failed to set screen to windowed mode.",
                                 NULL);
    }
    
    SDL_SetWindowSize(buffer->window, info.w - 80, info.h - 80);
    SDL_SetWindowPosition(buffer->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}