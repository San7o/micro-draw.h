// SPDX-License-Identifier: MIT

#define MICRO_DRAW_IMPLEMENTATION
#include "../micro-draw.h"

#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#include <time.h>

#define WIDTH  1280
#define HEIGHT 720
#define ITERATIONS 50
#define FPS    3

typedef struct {
  unsigned char* data;
  RGFW_window* win;
  unsigned char *screen;
  RGFW_surface *surface;
  double delta_time;
  double fps;
} Application;

int mandelbrot(int x, int y, int width, int height)
{
  double x_norm = (x / (double) width) * 2.47 - 2;
  double y_norm = (y / (double) height) * 2.24 - 1.12;

  int it = 0;
  double x_it = 0.0;
  double y_it = 0.0;
  while (x_it * x_it + y_it * y_it <= 2*2
         && it < ITERATIONS)
  {
    double tmp = x_it * x_it - y_it * y_it + x_norm;
    y_it = 2 * x_it * y_it + y_norm;
    x_it = tmp;
    it++;
  }
  
  return it;
}

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
    
  for (int y = 0; y < HEIGHT; ++y)
  {
    for (int x = 0; x < WIDTH; ++x)
    {
      int m = mandelbrot(x, y, WIDTH, HEIGHT);
      // Do some gradient mapping
      unsigned char color[] = {
        (m * 3) % 255,
        (m * 2) % 255,
        (m * 5) % 255,
        0xFF,
      };
      micro_draw_pixel(app->data, WIDTH, HEIGHT,
                       x, y,
                       color, MICRO_DRAW_RGBA8);
    }
  }
  
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
  
	app.win = RGFW_createWindow("mandelbrot_test",
                              0, 0, WIDTH, HEIGHT,
                              RGFW_windowCenter
                              | RGFW_windowNoResize
                              );
  // Note: X11 uses RGBA
  app.data = malloc(WIDTH*HEIGHT* micro_draw_get_channels(MICRO_DRAW_RGBA8));
  app.surface =
    RGFW_window_createSurface(app.win, (u8*)app.data, WIDTH, HEIGHT, RGFW_formatRGBA8);

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
  free(app.data);
  return 0;
}
