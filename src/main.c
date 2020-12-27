#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "include/language_layer.h"

#define GAME_TITLE "Game Title"
#define GAME_WIDTH 512
#define GAME_HEIGHT 288

typedef struct BUFFER_STRUCT {
  SDL_Window* window;
  SDL_Renderer* renderer;
} buffer_t;

typedef struct PLAYER_STRUCT {
  u32 x, y;
  u32 health;
  SDL_Texture* texture;
} player_t;

global_variable buffer_t global_buffer;
global_variable player_t global_player;
global_variable u64 global_total_rendered_frames;

internal bool 
initialize_player(buffer_t* buffer)
{
  global_player.x = 25;
  global_player.y = 25;
  global_player.health = 100;

  SDL_Surface* surface = SDL_LoadBMP("soldier_standing.bmpx");
  global_player.texture = SDL_CreateTextureFromSurface(buffer->renderer, surface);
  SDL_FreeSurface(surface);

  if (!global_player.texture)
  {
    printf("Failed to load player bmp.\n");
    return false;
  }
  
  return true;
}

internal void
player_render(player_t* player, buffer_t* buffer)
{
  SDL_Rect rect = { player->x, player->y, 16, 16 };
  SDL_RenderCopy(buffer->renderer, player->texture, NULL, &rect);
}

internal void
render_buffer_to_screen(buffer_t* buffer)
{
  SDL_RenderClear(buffer->renderer);

  // Insert Objects to Render
  SDL_SetRenderDrawColor(buffer->renderer, 0, 0, 0, 255);

  player_render(&global_player, buffer);
  
  SDL_RenderPresent(buffer->renderer);
}

int main(int argc, char* argv[])
{
  
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Error",
                             "Failed to init SDL.",
                              NULL);
    goto Exit;
  }
  
  /* Get Screen Size */

  SDL_DisplayMode display_mode_info;

  if (SDL_GetDesktopDisplayMode(0, &display_mode_info) != 0)
  {
   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Error",
                             "Failed to get displaymode.",
                             NULL);
    goto Exit;
  }

  global_buffer.window = SDL_CreateWindow(GAME_TITLE, 
                                          SDL_WINDOWPOS_UNDEFINED, 
                                          SDL_WINDOWPOS_UNDEFINED,
                                          display_mode_info.w,
                                          display_mode_info.h,
                                          SDL_WINDOW_SHOWN);
 
  if (!global_buffer.window) 
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Error",
                             "Failed to create window.",
                             NULL);
    goto Exit;
  }

  global_buffer.renderer = SDL_CreateRenderer(global_buffer.window, 
                                              -1,  // GPU thingy
                                               SDL_RENDERER_ACCELERATED 
                                               | SDL_RENDERER_PRESENTVSYNC); // Flags

  // Set logical render -- basically set buffer render res
  if (SDL_RenderSetLogicalSize(global_buffer.renderer, GAME_WIDTH, GAME_HEIGHT) != 0)
  {
    printf("Failed to set logical game res.\n");
    goto Exit;
  }

  if (SDL_SetWindowFullscreen(global_buffer.window, SDL_WINDOW_FULLSCREEN) != 0)
  {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Error",
                             "Failed to set window fullscreen.",
                             NULL);
    goto Exit;
  }

  if (!initialize_player(&global_buffer))
  {
    printf("Failed to initalize player!\n");
    goto Exit;
  }

  SDL_Event event;

  bool running = true;
  
  /* Main Game Loop */
  while (running)
  {
    /* Event Loop */
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        running = false;
    }

    /* Main Game Simulation */
    {
      render_buffer_to_screen(&global_buffer); 
      
    }

    global_total_rendered_frames++;
  }
  

Exit:
  SDL_DestroyTexture(global_player.texture);
  SDL_DestroyRenderer(global_buffer.renderer);
  SDL_DestroyWindow(global_buffer.window);
  SDL_Quit();
  
  return 0; 
}
