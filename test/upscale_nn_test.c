// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define MICRO_DRAW_IMPLEMENTATION
#include "../micro-draw.h"

#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#include <time.h>

#define SCREEN_WIDTH  500
#define SCREEN_HEIGHT 500
#define FRAME_WIDTH   3
#define FRAME_HEIGHT  3
#define ITERATIONS    50
#define FPS           3

typedef struct {
  unsigned char* screen_data;
  RGFW_window* win;
  unsigned char *frame_data;
  unsigned char *screen;
  RGFW_surface *surface;
  double delta_time;
  double fps;
} Application;

void loop(void *arg)
{
  Application *app = (Application*) arg;
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

  printf("FPS: %f\n", 1 / app->delta_time);
  app->delta_time = 0;

  // Render frame
  unsigned char color_white[] = {255, 255, 255, 255};
  unsigned char color_red[] = {255, 0, 0, 255};
  for (int y = 0; y < FRAME_HEIGHT; ++y)
    for (int x = 0; x < FRAME_WIDTH; ++x)
    {
      if (y == FRAME_HEIGHT / 2 && x == FRAME_WIDTH / 2)
      {
        micro_draw_pixel(app->frame_data, FRAME_WIDTH, FRAME_HEIGHT,
                         x, y,
                         color_red, MICRO_DRAW_RGBA8);
      }
      else
      {
        micro_draw_pixel(app->frame_data, FRAME_WIDTH, FRAME_HEIGHT,
                         x, y,
                         color_white, MICRO_DRAW_RGBA8);
      }
    }
  
  // Upscale
  for (int y = 0; y < SCREEN_HEIGHT; ++y)
  {
    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
      int x_frame = (x * FRAME_WIDTH) / (double)SCREEN_WIDTH;
      int y_frame = (y * FRAME_HEIGHT) / (double)SCREEN_HEIGHT;
      unsigned char *color;
      micro_draw_get_color(app->frame_data, FRAME_WIDTH, FRAME_HEIGHT,
                           x_frame, y_frame, &color, MICRO_DRAW_RGBA8);
      micro_draw_pixel(app->screen_data, SCREEN_WIDTH, SCREEN_HEIGHT,
                       x, y,
                       color, MICRO_DRAW_RGBA8);
    }
  }

  micro_draw_scaled(app->frame_data, FRAME_WIDTH, FRAME_HEIGHT, MICRO_DRAW_RGBA8,
                    app->screen_data, SCREEN_WIDTH, SCREEN_HEIGHT, MICRO_DRAW_RGBA8);
  
  RGFW_window_blitSurface(app->win, app->surface);
    
  #ifndef __EMSCRIPTEN__
  }
  #endif
  
  clock_t frame_end = clock();
  app->delta_time += (double)(frame_end - frame_start) / CLOCKS_PER_SEC;
  return;
}

int main(void)
{
  Application app;
  
	app.win = RGFW_createWindow("upscale nn test",
                              0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                              RGFW_windowCenter
                              | RGFW_windowNoResize
                              );
  // Note: X11 uses RGBA

  app.frame_data = malloc(FRAME_WIDTH*FRAME_HEIGHT* micro_draw_get_channels(MICRO_DRAW_RGBA8));
  app.screen_data = malloc(SCREEN_WIDTH*SCREEN_HEIGHT* micro_draw_get_channels(MICRO_DRAW_RGBA8));
  app.surface =
    RGFW_window_createSurface(app.win, (u8*)app.screen_data, SCREEN_WIDTH, SCREEN_HEIGHT, RGFW_formatRGBA8);

  RGFW_window_setExitKey(app.win, RGFW_escape);

  app.delta_time = 0;
  app.fps = FPS;
  
  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(loop, &app, app.fps, 1);
  #else
  while (RGFW_window_shouldClose(app.win) == RGFW_FALSE) loop(&app);
  #endif // __EMSCRIPTEN__

  RGFW_surface_free(app.surface);
	RGFW_window_close(app.win);
  free(app.screen_data);
  free(app.frame_data);
  return 0;
}
