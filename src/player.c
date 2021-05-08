#include "include/input.h"

internal bool
initialize_player(game_t *game, buffer_t *buffer)
{
    game->player.x = (GAME_WIDTH / 2) - GAME_SIZE;
    game->player.y = (GAME_HEIGHT / 2) - GAME_SIZE;
    game->player.health = 100;
    game->player.moving = false;
    game->player.direction_index = 0;
    game->player.is_shooting = false;
    
    game->player.player_render_rect.x = 0;
    game->player.player_render_rect.y = 0;
    game->player.player_render_rect.w = GAME_SIZE;
    game->player.player_render_rect.h = GAME_SIZE;
    
    SDL_Surface *surface = IMG_Load("Assets/player_sprites.png");
    game->player.sheet_texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!game->player.sheet_texture)
    {
        printf("Failed to load sprite sheet png.\n");
        return false;
    }
    
    SDL_QueryTexture(game->player.sheet_texture,
                     NULL,
                     NULL,
                     &game->player.text_rect.w,
                     &game->player.text_rect.h);
    
    return true;
}

internal void
player_input(game_t *game, game_input_t *input, SDL_MouseButtonEvent m_event)
{
    bool pressed = false;
    
    const u8 *state = SDL_GetKeyboardState(NULL);
    
    /* Player Movement Code */
    if (game->game_state.playing)
    {
        if (state[SDL_SCANCODE_W] | input->gamepad.up)
        {
            game->player.y--;
            pressed = true;
            
            game->player.direction_index = PLAYER_UP;
        }
        
        if (state[SDL_SCANCODE_A] | input->gamepad.left)
        {
            game->player.x--;
            pressed = true;
            
            game->player.direction_index = PLAYER_LEFT;
        }
        
        if (state[SDL_SCANCODE_S] | input->gamepad.down)
        {
            game->player.y++;
            pressed = true;
            
            game->player.direction_index = PLAYER_DOWN;
        }
        
        if (state[SDL_SCANCODE_D] | input->gamepad.right)
        {
            game->player.x++;
            pressed = true;
            
            game->player.direction_index = PLAYER_RIGHT;
        }
        
        /* Funnel Player Ability Code */
        if (mouse_input(game, &m_event) | input->gamepad.a_button)
            game->player.is_shooting = true;
        else
            game->player.is_shooting = false;
        
        // Controller Buttons
        if (input->gamepad.a_button) {
            printf("A button pressed.\n");
        }
    }
    
    if (pressed)
        game->player.moving = true;
    else
        game->player.moving = false;
    
    pressed = !pressed;
}

internal void
player_sprite_animations(game_t *game)
{
    u32 ticks = SDL_GetTicks();
    u32 sprite = (ticks / 100) % 3;
    
    // If moving set animation in proper direction
    if (game->player.moving && game->player.direction_index == PLAYER_DOWN)
    {
        game->player.player_render_rect.y = 0;
        game->player.player_render_rect.x = sprite << 4;
    }
    
    if (game->player.moving && game->player.direction_index == PLAYER_LEFT)
    {
        game->player.player_render_rect.y = 16;
        game->player.player_render_rect.x = sprite << 4;
    }
    
    if (game->player.moving && game->player.direction_index == PLAYER_RIGHT)
    {
        game->player.player_render_rect.y = 32;
        game->player.player_render_rect.x = sprite << 4;
    }
    
    if (game->player.moving && game->player.direction_index == PLAYER_UP)
    {
        game->player.player_render_rect.y = 48;
        game->player.player_render_rect.x = sprite << 4;
    }
    
    // If not moving set idle to the direction last moving
    if (!game->player.moving && game->player.direction_index == PLAYER_DOWN)
    {
        game->player.player_render_rect.x = 0;
        game->player.player_render_rect.y = 0;
    }
    
    if (!game->player.moving && game->player.direction_index == PLAYER_LEFT)
    {
        game->player.player_render_rect.x = 0;
        game->player.player_render_rect.y = 16;
    }
    
    if (!game->player.moving && game->player.direction_index == PLAYER_RIGHT)
    {
        game->player.player_render_rect.x = 0;
        game->player.player_render_rect.y = 32;
    }
    
    if (!game->player.moving && game->player.direction_index == PLAYER_UP)
    {
        game->player.player_render_rect.x = 0;
        game->player.player_render_rect.y = 48;
    }
}

internal void
player_render(game_t *game, buffer_t *buffer)
{
    SDL_Rect rect = { game->player.x, game->player.y,
        game->player.text_rect.w / 3, game->player.text_rect.h / 4 };
    
    player_sprite_animations(game);
    
    SDL_RenderCopy(buffer->renderer, game->player.sheet_texture, &game->player.player_render_rect, &rect);
}
