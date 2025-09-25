// SPDX-License-Identifier: MIT

#define MICRO_DRAW_IMPLEMENTATION
#include "../micro-draw.h"

#define GAME_OF_LIFE_IMPLEMENTATION
#include "game-of-life.h"

#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#include <time.h>

#define GRID_WIDTH 25
#define GRID_HEIGHT 25
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

void draw_cells(bool *universe, int universe_width, int universe_height,
                unsigned char *screen, int screen_width, int screen_height,
                unsigned char *color_alive, unsigned char *color_dead,
                MicroDrawPixel pixel)
{
  int square_width = screen_width / universe_width;
  int square_height = screen_height / universe_height;

  for (int row = 0; row < universe_height; ++row)
    for (int col = 0; col < universe_width; ++col)
    {
      if (universe[row * universe_width + col])
      {
        
        micro_draw_fill_rect(screen, screen_width, screen_height,
                             square_width * col, square_height * row,
                             square_width, square_height,
                             color_alive, pixel);
      }
      else
      {
        micro_draw_fill_rect(screen, screen_width, screen_height,
                             square_width * col, square_height * row,
                             square_width, square_height,
                             color_dead, pixel);
      }
    }
      
  return;
}

int main(void)
{
  // Acorn
  bool universe_buf[GRID_WIDTH*GRID_HEIGHT] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };
  bool scratch_buf[GRID_WIDTH*GRID_HEIGHT];

  // Arrays do not work, you need to use pointers
  bool *universe = universe_buf;
  bool *scratch  = scratch_buf;
  
  RGFW_window* win = RGFW_createWindow("game of life",
                                       0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                                       RGFW_windowCenter
                                       | RGFW_windowNoResize);
  RGFW_event event;
  // Note: X11 uses RGBA
  unsigned char* screen = malloc(SCREEN_WIDTH*SCREEN_HEIGHT
                                 *micro_draw_get_channels(MICRO_DRAW_RGBA8));

  RGFW_surface *surface =
    RGFW_window_createSurface(win, (u8*)screen,
                              SCREEN_WIDTH, SCREEN_HEIGHT, RGFW_formatRGBA8);

  RGFW_window_setExitKey(win, RGFW_escape);

  double delta_time = 0;
  double fps = 5;
  unsigned char color_white[4] = {255, 255, 255, 255};
  unsigned char color_black[4] = {0, 0, 0, 255};
  
  while (RGFW_window_shouldClose(win) == RGFW_FALSE)
  {
    clock_t frame_start = clock();

    while (RGFW_window_checkEvent(win, &event))
    {
      if (event.type == RGFW_quit)
        break;
    }

    if (delta_time > 1 / fps)
    {
      // Draw
      delta_time = 0;

      game_of_life_simulate(&universe, &scratch,
                            GRID_WIDTH, GRID_HEIGHT);
      
      micro_draw_clear(screen, SCREEN_WIDTH, SCREEN_HEIGHT,
                       color_white, MICRO_DRAW_RGBA8);  

      draw_cells(universe, GRID_WIDTH, GRID_HEIGHT,
                 screen, SCREEN_WIDTH, SCREEN_HEIGHT,
                 color_black, color_white, MICRO_DRAW_RGBA8);

      micro_draw_grid(screen, SCREEN_WIDTH, SCREEN_HEIGHT,
                      GRID_WIDTH, GRID_HEIGHT,
                      color_black, MICRO_DRAW_RGBA8);
      
      RGFW_window_blitSurface(win, surface);
    }

    clock_t frame_end = clock();
    delta_time += (double)(frame_end - frame_start) / CLOCKS_PER_SEC;
  }

  RGFW_surface_free(surface);
  free(screen);
  RGFW_window_close(win);
  
  return 0;
}
