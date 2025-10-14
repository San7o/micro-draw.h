// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define MICRO_DRAW_IMPLEMENTATION
#include "../micro-draw.h"

#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define WIDTH  800
#define HEIGHT 500

int main(void)
{
	RGFW_window* win = RGFW_createWindow("fill_triangle_test",
                                       0, 0, WIDTH, HEIGHT,
                                       RGFW_windowCenter
                                       | RGFW_windowNoResize);
	RGFW_event event;
  // Note: X11 uses RGBA
  unsigned char* data = malloc(WIDTH*HEIGHT* micro_draw_get_channels(MICRO_DRAW_RGBA8));

  RGFW_surface *surface =
    RGFW_window_createSurface(win, (u8*)data, WIDTH, HEIGHT, RGFW_formatRGBA8);

  RGFW_window_setExitKey(win, RGFW_escape);
  
	while (RGFW_window_shouldClose(win) == RGFW_FALSE)
  {
		while (RGFW_window_checkEvent(win, &event))
    {
      if (event.type == RGFW_quit)
        break;
    }

    unsigned char color_green[4] = {0, 255, 0, 255};
    micro_draw_clear(data, WIDTH, HEIGHT, color_green, MICRO_DRAW_RGBA8);  

    
    int a_x = WIDTH / 2; int a_y = HEIGHT / 3;
    int b_x = WIDTH / 3 * 2; int b_y = HEIGHT / 3 * 2;
    int c_x = WIDTH / 3;  int c_y = HEIGHT / 3 * 2;
    unsigned char color_blue[4] = {0, 0, 255, 255};
    micro_draw_fill_triangle(data, WIDTH, HEIGHT,
                             a_x, a_y,
                             b_x, b_y,
                             c_x, c_y,
                             color_blue, MICRO_DRAW_RGBA8);  
  
    RGFW_window_blitSurface(win, surface);
	}

  RGFW_surface_free(surface);
  free(data);
	RGFW_window_close(win);
  return 0;
}
