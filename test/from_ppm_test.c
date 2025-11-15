// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#define MICRO_DRAW_IMPLEMENTATION
#define MICRO_DRAW_PPM
#include "../micro-draw.h"

#include <stdio.h>

int main(void)
{
  unsigned char* data;
  int width, height;
  MicroDrawPixel pixel;

  MicroDrawError error =
          micro_draw_from_ppm("/tmp/test-to-ppm.ppm",
          &data,
          &width, &height,
          &pixel);
  if (error != MICRO_DRAW_OK)
  {
    fprintf(stderr, "Error %d opening ppm file\n", error);
    return 1;
  }

  //printf("data_height: %d, data_width: %d, color_max: %d\n",
  //       height, width, color_max);
  
  free(data);
  
  return 0;
}
