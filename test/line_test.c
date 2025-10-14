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
	RGFW_window* win = RGFW_createWindow("line_test",
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

    unsigned char color_white[4] = {255, 255, 255, 255};
    micro_draw_clear(data, WIDTH, HEIGHT, color_white, MICRO_DRAW_RGBA8);  

    unsigned char color_red[4] = {255, 0, 0, 255};
    
    // slope
    micro_draw_line(data, WIDTH, HEIGHT,
                    WIDTH / 3, HEIGHT / 3,
                    WIDTH / 3 * 2, HEIGHT / 3 * 2,
                    color_red, MICRO_DRAW_RGBA8);

    // Horizontal
    micro_draw_line(data, WIDTH, HEIGHT,
                    WIDTH / 3, HEIGHT / 3,
                    WIDTH / 3 * 2, HEIGHT / 3,
                    color_red, MICRO_DRAW_RGBA8);

    // Vertical
    micro_draw_line(data, WIDTH, HEIGHT,
                    WIDTH / 3, HEIGHT / 3,
                    WIDTH / 3, HEIGHT / 3 * 2,
                    color_red, MICRO_DRAW_RGBA8);
    


    
    RGFW_window_blitSurface(win, surface);
	}

  RGFW_surface_free(surface);
  free(data);
	RGFW_window_close(win);
  return 0;
}
