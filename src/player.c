#include "include/input.h"

internal bool
initialize_player(player_t *player, game_t *game, buffer_t *buffer)
{
    player->x = (game->render_res_w / 2) - GAME_SIZE;
    player->y = (game->render_res_h / 2) - GAME_SIZE;
    player->health = 100;
    player->moving = false;
    player->direction_index = 0;
    player->is_shooting = false;
    
    player->player_render_rect.x = 0;
    player->player_render_rect.y = 0;
    player->player_render_rect.w = GAME_SIZE;
    player->player_render_rect.h = GAME_SIZE;
    
    SDL_Surface *surface = IMG_Load("Assets/player_sprites.png");
    player->sheet_texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!player->sheet_texture)
    {
        printf("Failed to load sprite sheet png.\n");
        return false;
    }
    
    SDL_QueryTexture(player->sheet_texture,
                     NULL,
                     NULL,
                     &player->text_rect.w,
                     &player->text_rect.h);
    
    return true;
}

internal void
player_input(player_t *player,
             game_t *game,
             game_input_t *input,
             SDL_MouseButtonEvent m_event)
{
    bool pressed = false;
    
    const u8 *state = SDL_GetKeyboardState(NULL);
    
    /* Player Movement Code */
    if (game->game_state.playing)
    {
        if (state[SDL_SCANCODE_W] | input->gamepad.up)
        {
            player->y--;
            pressed = true;
            
            player->direction_index = PLAYER_UP;
        }
        
        if (state[SDL_SCANCODE_A] | input->gamepad.left)
        {
            player->x--;
            pressed = true;
            
            player->direction_index = PLAYER_LEFT;
        }
        
        if (state[SDL_SCANCODE_S] | input->gamepad.down)
        {
            player->y++;
            pressed = true;
            
            player->direction_index = PLAYER_DOWN;
        }
        
        if (state[SDL_SCANCODE_D] | input->gamepad.right)
        {
            player->x++;
            pressed = true;
            
            player->direction_index = PLAYER_RIGHT;
        }
        
        /* Funnel Player Ability Code */
        if (mouse_input(game, &m_event) | input->gamepad.a_button)
            player->is_shooting = true;
        else
            player->is_shooting = false;
        
        // Controller Buttons
        if (input->gamepad.a_button) {
            printf("A button pressed.\n");
        }
    }
    
    if (pressed)
        player->moving = true;
    else
        player->moving = false;
    
    pressed = !pressed;
}

internal void
player_sprite_animations(player_t *player)
{
    u32 ticks = SDL_GetTicks();
    u32 sprite = (ticks / 100) % 3;
    
    // If moving set animation in proper direction
    if (player->moving && player->direction_index == PLAYER_DOWN)
    {
        player->player_render_rect.y = 0;
        player->player_render_rect.x = sprite << 4;
    }
    
    if (player->moving && player->direction_index == PLAYER_LEFT)
    {
        player->player_render_rect.y = 16;
        player->player_render_rect.x = sprite << 4;
    }
    
    if (player->moving && player->direction_index == PLAYER_RIGHT)
    {
        player->player_render_rect.y = 32;
        player->player_render_rect.x = sprite << 4;
    }
    
    if (player->moving && player->direction_index == PLAYER_UP)
    {
        player->player_render_rect.y = 48;
        player->player_render_rect.x = sprite << 4;
    }
    
    // If not moving set idle to the direction last moving
    if (!player->moving && player->direction_index == PLAYER_DOWN)
    {
        player->player_render_rect.x = 0;
        player->player_render_rect.y = 0;
    }
    
    if (!player->moving && player->direction_index == PLAYER_LEFT)
    {
        player->player_render_rect.x = 0;
        player->player_render_rect.y = 16;
    }
    
    if (!player->moving && player->direction_index == PLAYER_RIGHT)
    {
        player->player_render_rect.x = 0;
        player->player_render_rect.y = 32;
    }
    
    if (!player->moving && player->direction_index == PLAYER_UP)
    {
        player->player_render_rect.x = 0;
        player->player_render_rect.y = 48;
    }
}

internal void
player_render(player_t *player, buffer_t *buffer)
{
    SDL_Rect rect = { player->x, player->y,
        player->text_rect.w / 3, player->text_rect.h / 4 };
    
    player_sprite_animations(player);
    
    SDL_RenderCopy(buffer->renderer, player->sheet_texture, &player->player_render_rect, &rect);
}
