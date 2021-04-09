#include "include/enemy.h"
#include <stdlib.h>

internal enemy_t *
initialize_enemy(buffer_t *buffer)
{
    enemy_t *enemy = calloc(1, sizeof(struct ENEMY_STRUCT));

    enemy->x = 25;
    enemy->y = 25;
    enemy->speed = 5.f;

    enemy->enemy_render_rect.x = 0;
    enemy->enemy_render_rect.y = 0;
    enemy->enemy_render_rect.w = GAME_SIZE;
    enemy->enemy_render_rect.h = GAME_SIZE;

    SDL_Surface *surface = IMG_Load("Assets/new_sheet.png");
    enemy->texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
    SDL_FreeSurface(surface);

    if (!enemy->texture)
    {
        printf("Failed to load sprite sheet png.\n");
        return (enemy_t*) 0;
    }

    SDL_QueryTexture(enemy->texture,
                     NULL,
                     NULL,
                     &enemy->text_rect.w,
                     &enemy->text_rect.h);

    return enemy;
}

internal void
render_enemy(buffer_t *buffer, enemy_t *enemy)
{
    SDL_Rect rect = { enemy->x, enemy->y,
        enemy->text_rect.w / 3, enemy->text_rect.h / 4 };

    SDL_RenderCopy(buffer->renderer, enemy->texture,
                   &enemy->enemy_render_rect, &rect);
}
