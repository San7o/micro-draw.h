// SPDX-License-Identifier: MIT

#define MICRO_DRAW_IMPLEMENTATION
#define MICRO_DRAW_PPM
#include "../micro-draw.h"

#define WIDTH  800
#define HEIGHT 500

#include <stdlib.h>
#include <assert.h>

int main(void)
{
  int channels = micro_draw_get_channels(MICRO_DRAW_RGBA8);
  int channel_size = micro_draw_get_channel_size(MICRO_DRAW_RGBA8);
  int data_size = WIDTH * HEIGHT * channels * channel_size;
  unsigned char* data = malloc(data_size);

  unsigned char color_white[4] = {255, 255, 255, 255};
  micro_draw_clear(data, WIDTH, HEIGHT, color_white, MICRO_DRAW_RGBA8);  

    
  int radius = 100;
  unsigned char color_red[4] = {255, 0, 0, 255};
  micro_draw_fill_circle(data, WIDTH, HEIGHT,
                         WIDTH / 2, HEIGHT / 2,
                         radius, color_red, MICRO_DRAW_RGBA8);  
  
  assert(micro_draw_to_ppm("/tmp/test-to-ppm.ppm",
                           data,
                           WIDTH, HEIGHT,
                           MICRO_DRAW_RGBA8) == MICRO_DRAW_OK);
  
  free(data);
  return 0;
}
