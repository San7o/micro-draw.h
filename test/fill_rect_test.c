// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define MICRO_DRAW_IMPLEMENTATION
#include "../micro-draw.h"

#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#define WIDTH  500
#define HEIGHT 500

int main(void)
{
	RGFW_window* win = RGFW_createWindow("fill_rect_test",
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

    unsigned char color[4] = {255, 0, 0, 255};
    micro_draw_fill_rect(data, WIDTH, HEIGHT,
                         70, 100,
                         100, 200, color, MICRO_DRAW_RGBA8);

    micro_draw_fill_rect(data, WIDTH, HEIGHT, 300, 100, 100,
                         200, color, MICRO_DRAW_RGBA8);
    
    RGFW_window_blitSurface(win, surface);
	}

  RGFW_surface_free(surface);
  free(data);
	RGFW_window_close(win);
  return 0;
}
