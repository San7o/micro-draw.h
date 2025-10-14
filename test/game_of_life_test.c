// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define MICRO_DRAW_IMPLEMENTATION
#include "../micro-draw.h"

#define GAME_OF_LIFE_IMPLEMENTATION
#include "game-of-life.h"

#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#include <time.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define GRID_WIDTH 25
#define GRID_HEIGHT 25
#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

typedef struct {
  bool *universe;
  bool *scratch;
  RGFW_window* win;
  unsigned char *screen;
  RGFW_surface *surface;
  double delta_time;
  double fps;
} Application;

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

void loop(void *arg)
{
  Application *app = (Application*) arg;
  unsigned char color_white[4] = {255, 255, 255, 255};
  unsigned char color_black[4] = {0, 0, 0, 255};
  
  RGFW_event event;
  clock_t frame_start = clock();

  while (RGFW_window_checkEvent(app->win, &event))
  {
    if (event.type == RGFW_quit)
      break;
  }

  #ifndef __EMSCRIPTEN__
  if (app->delta_time > 1 / app->fps)
  {
  #endif
    // Draw
    app->delta_time = 0;

    game_of_life_simulate(&app->universe, &app->scratch,
                          GRID_WIDTH, GRID_HEIGHT);
      
    micro_draw_clear(app->screen, SCREEN_WIDTH, SCREEN_HEIGHT,
                     color_white, MICRO_DRAW_RGBA8);  

    draw_cells(app->universe, GRID_WIDTH, GRID_HEIGHT,
               app->screen, SCREEN_WIDTH, SCREEN_HEIGHT,
               color_black, color_white, MICRO_DRAW_RGBA8);

    micro_draw_grid(app->screen, SCREEN_WIDTH, SCREEN_HEIGHT,
                    GRID_WIDTH, GRID_HEIGHT,
                    color_black, MICRO_DRAW_RGBA8);
      
    RGFW_window_blitSurface(app->win, app->surface);
    
  #ifndef __EMSCRIPTEN__
   }
  #endif

  clock_t frame_end = clock();
  app->delta_time += (double)(frame_end - frame_start) / CLOCKS_PER_SEC;
}

int main(void)
{
  Application app;
  
  // Acorn
  bool universe[GRID_WIDTH*GRID_HEIGHT] = {
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
  bool scratch[GRID_WIDTH*GRID_HEIGHT];
  app.universe = universe;
  app.scratch = scratch;
  
  app.win = RGFW_createWindow("game of life",
                              0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                              RGFW_windowCenter
                              | RGFW_windowNoResize);
  // Note: X11 uses RGBA
  app.screen = malloc(SCREEN_WIDTH*SCREEN_HEIGHT
                      *micro_draw_get_channels(MICRO_DRAW_RGBA8));

  app.surface =
    RGFW_window_createSurface(app.win, (u8*)app.screen,
                              SCREEN_WIDTH, SCREEN_HEIGHT, RGFW_formatRGBA8);

  RGFW_window_setExitKey(app.win, RGFW_escape);

  app.delta_time = 0;
  app.fps = 3;

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(loop, &app, app.fps, 1);
  #else
  while (RGFW_window_shouldClose(app.win) == RGFW_FALSE) loop(&app);
  #endif // __EMSCRIPTEN__

  RGFW_surface_free(app.surface);
  free(app.screen);
  RGFW_window_close(app.win);
  
  return 0;
}
