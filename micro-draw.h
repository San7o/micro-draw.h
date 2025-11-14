//////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
//
// micro-draw.h
// ============
//
// Header-only software renderer in C99.
//
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// License: MIT
//
//
// Features
// --------
//
//  - draw lines
//  - draw rectangles
//  - draw circles
//  - draw triangles
//  - draw grids
//  - RGBA, easy to add other formats
//  - PPM file reading and writing
//
//
// Usage
// -----
//
// Do this:
//
//   #define MICRO_DRAW_IMPLEMENTATION
//
// before you include this file in *one* C or C++ file to create the
// implementation.
//
// i.e. it should look like this:
//
//   #include ...
//   #include ...
//   #include ...
//   #define MICRO_DRAW_IMPLEMENTATION
//   #include "micro-draw.h"
//
// You can tune the library by #defining certain values. See the
// "Config" comments under "Configuration" below.
//
// To enable PPM related functions, you need to #define MIRO_DRAW_PPM.
// More info: https://en.wikipedia.org/wiki/Netpbm
//
// The usage is quite straight forward: you supply a data buffer to a
// micro-draw.h function which will fill the pixels accordingly. For
// example, you can use this buffer to render a frame on screen, or to
// save it as an image file, or do whatever you want.
//
// Check out some examples in the `test` directory.
//
//
// Code
// ----
//
// The official git repository of micro-draw.h is hosted at:
//
//     https://github.com/San7o/micro-draw.h
//
// This is part of a bigger collection of header-only C99 libraries
// called "micro-headers", contributions are welcome:
//
//     https://github.com/San7o/micro-headers
//
//
// TODO
// ----
//
//  - Implement micro_draw_from_ppm
//  - Add text rendering
//

#ifndef MICRO_DRAW
#define MICRO_DRAW

#define MICRO_DRAW_MAJOR 0
#define MICRO_DRAW_MINOR 1

#ifdef __cplusplus
extern "C" {
#endif

//
// Configuration
//

// Config: enable PPM format reading / writing with MICRO_DRAW_PPM
#if 0
  #define MICRO_DRAW_PPM
#endif

// Config: Prefix for all functions
// For function inlining, set this to `static inline` and then define
// the implementation in all the files
#ifndef MICRO_DRAW_DEF
  #define MICRO_DRAW_DEF extern
#endif

//
// Macros
//

#define MICRO_DRAW_ABS(x) (((x) >= 0) ? (x) : -(x))

//
// Types
//

#include <stddef.h>

typedef enum {
  MICRO_DRAW_RGBA8 = 0,
  _MICRO_DRAW_PIXEL_MAX,
} MicroDrawPixel;

typedef enum {
  MICRO_DRAW_OK = 0,
  MICRO_DRAW_ERROR_OPEN_FILE,
  MICRO_DRAW_ERROR_INVALID_MAGIC_NUMBER,
  _MICRO_DRAW_ERROR_MAX,
} MicroDrawError;

#define MICRO_DRAW_FONT_HEIGHT 6
#define MICRO_DRAW_FONT_WIDTH 5
extern unsigned char
micro_draw_font[128][MICRO_DRAW_FONT_HEIGHT][MICRO_DRAW_FONT_WIDTH];
  
//
// Function declarations
//

// Return the number of channels of a pixel type
MICRO_DRAW_DEF size_t micro_draw_get_channels(MicroDrawPixel pixel);

// Returns the number of bytes of a single channel if a pixel type
MICRO_DRAW_DEF size_t micro_draw_get_channel_size(MicroDrawPixel pixel);

MICRO_DRAW_DEF void
micro_draw_pixel(unsigned char* data,
                 int data_width, int data_height,
                 int x, int y,
                 unsigned char* color, MicroDrawPixel pixel);

MICRO_DRAW_DEF void
micro_draw_line(unsigned char* data,
                int data_width, int data_height,
                int a_x, int a_y,
                int b_x, int b_y,
                unsigned char* color, MicroDrawPixel pixel);

MICRO_DRAW_DEF void
micro_draw_clear(unsigned char* data,
                 int data_width, int data_height,
                 unsigned char *color, MicroDrawPixel pixel);

MICRO_DRAW_DEF void
micro_draw_fill_rect(unsigned char* data,
                     int data_width, int data_height,
                     int x, int y, int w, int h,
                     unsigned char *color, MicroDrawPixel pixel);

MICRO_DRAW_DEF void
micro_draw_fill_circle(unsigned char* data,
                       int data_width, int data_height,
                       int center_x, int center_y, int radius,
                       unsigned char *color, MicroDrawPixel pixel);

MICRO_DRAW_DEF void
micro_draw_fill_triangle(unsigned char *data,
                         int data_width, int data_height,
                         int a_x, int a_y,
                         int b_x, int b_y,
                         int c_x, int c_y,
                         unsigned char *color, MicroDrawPixel pixel);

MICRO_DRAW_DEF void
micro_draw_grid(unsigned char* data,
                int data_width, int data_height,
                int columns, int rows,
                unsigned char* color, MicroDrawPixel pixel);


MICRO_DRAW_DEF void
micro_draw_get_color(unsigned char* data,
                     int data_width, int data_height,
                     int x, int y,
                     unsigned char** color, MicroDrawPixel pixel);

MICRO_DRAW_DEF void
micro_draw_scaled(unsigned char* source_data,
                  int source_data_width, int souce_data_height,
                  MicroDrawPixel src_pixel,
                  unsigned char* dest_data,
                  int dest_data_width, int dest_data_height,
                  MicroDrawPixel dest_pixel);
  
MICRO_DRAW_DEF void
micro_draw_text(unsigned char* data,
                int data_width, int data_height,
                int text_x, int text_y, int text_size, char* text,
                unsigned char* color, MicroDrawPixel pixel);

#ifdef MICRO_DRAW_PPM

MICRO_DRAW_DEF MicroDrawError
micro_draw_to_ppm(const char *filename,
                  unsigned char *data,
                  int data_width, int data_height,
                  MicroDrawPixel pixel);


MICRO_DRAW_DEF MicroDrawError
micro_draw_from_ppm(const char *filename,
                    unsigned char **data,
                    int *data_width, int *data_height,
                    MicroDrawPixel *pixel);

#endif // MICRO_DRAW_PPM

//
// Implementation
//

#ifdef MICRO_DRAW_IMPLEMENTATION

#include <string.h>
#include <assert.h>

_Static_assert(_MICRO_DRAW_PIXEL_MAX == 1,
               "Updated MicroDrawPixel, should also update micro_draw_get_channels");
MICRO_DRAW_DEF size_t micro_draw_get_channels(MicroDrawPixel pixel)
{
  switch(pixel)
  {
  case MICRO_DRAW_RGBA8:
    return 4;
  default:
    break;
  }

  return 0;
}

_Static_assert(_MICRO_DRAW_PIXEL_MAX == 1,
               "Updated MicroDrawPixel, should also update micro_draw_get_channel_size");
MICRO_DRAW_DEF size_t micro_draw_get_channel_size(MicroDrawPixel pixel)
{
  switch(pixel)
  {
  case MICRO_DRAW_RGBA8:
    return 1;
  default:
    break;
  }

  return 0;
}

MICRO_DRAW_DEF void
micro_draw_pixel(unsigned char* data,
                 int data_width, int data_height,
                 int x, int y,
                 unsigned char* color, MicroDrawPixel pixel)
{
  if (x >= data_width || x < 0 || y >= data_height || y < 0) return;

  int channel_size = micro_draw_get_channel_size(pixel); // bytes
  size_t channels = micro_draw_get_channels(pixel);
  int index = (y * data_width + x) * channels * channel_size;
  
  memcpy(&data[index], color, channels * channel_size);

  return;
}

MICRO_DRAW_DEF void
micro_draw_line(unsigned char* data,
                int data_width, int data_height,
                int a_x, int a_y,
                int b_x, int b_y,
                unsigned char* color, MicroDrawPixel pixel)
{
  // Line equation
  double m = (a_y - b_y) / (double)(a_x - b_x);
  double q = a_y - m * a_x;
  int is_steep = (m > 1 || m < -1);

  if (is_steep)
  {
    // Transpose the image
    int tmp = a_x;
    a_x = a_y;
    a_y = tmp;

    tmp = b_x;
    b_x = b_y;
    b_y = tmp;

    m = (a_y - b_y) / (double)(a_x - b_x);
    q = a_y - m * a_x;
  }

  int start, end;
  if (a_x > b_x)
  {
    start = b_x;
    end = a_x;
  }
  else
  {
    start = a_x;
    end = b_x;
  }

  if (is_steep)
  {
    // Iterate by columns
    for (int p_x = start; p_x < end; ++p_x)
    {
      int p_y = m * p_x + q;
      // Retranspose the image
      micro_draw_pixel(data, data_width, data_height,
                       p_y, p_x, color, pixel);
    }
  }
  else
  {
    // Transposed
    for (int p_x = start; p_x < end; ++p_x)
    {
      int p_y = m * p_x + q;
      micro_draw_pixel(data, data_width, data_height,
                       p_x, p_y, color, pixel);
    }
  }
  
  return;
}

MICRO_DRAW_DEF void
micro_draw_clear(unsigned char* data,
                 int data_width, int data_height,
                 unsigned char *color, MicroDrawPixel pixel)
{
  for (int row = 0; row < data_height; ++row)
    for (int col = 0; col < data_width; ++col)
      micro_draw_pixel(data, data_width, data_height,
                       col, row, color, pixel);
}

MICRO_DRAW_DEF void
micro_draw_fill_rect(unsigned char* data,
                     int data_width, int data_height,
                     int x, int y, int w, int h,
                     unsigned char *color, MicroDrawPixel pixel)
{
  for (int row = y; row < h + y && row < data_height; ++row)
  {
    for (int col = x; col < w + x && col < data_width; ++col)
    {
      if (row < 0 || col < 0) continue;
      micro_draw_pixel(data, data_width, data_height,
                       col, row, color, pixel);
    }
  }
  
  return;
}

MICRO_DRAW_DEF void
micro_draw_fill_circle(unsigned char* data,
                       int data_width, int data_height,
                       int center_x, int center_y, int radius,
                       unsigned char *color, MicroDrawPixel pixel)
{
  for (int row = center_y - radius; row < center_y + radius && row < data_height; ++row)
  {
    for (int col = center_x - radius; col < center_x + radius && col < data_width; ++col)
    {
      if (row < 0 || col < 0) continue;
      
      int dx = MICRO_DRAW_ABS(col - center_x);
      int dy = MICRO_DRAW_ABS(row - center_y);
      if (dx*dx + dy*dy > radius*radius) continue;
      
      micro_draw_pixel(data, data_width, data_height,
                       col, row, color, pixel);
    }
  }

  return;
}

// Get the orientation of three 2D points (a, b, c).
//
// This computes the determinant:
//
// det( [[ a_x, a_y, 1 ],
//       [ b_x, b_y, 1 ],
//       [ c_x, c_y, 1 ]] )
//
// Algebraically, this is equal to
//
//   (b_x - a_x) * (c_y - a_y) - (b_y - a_y) * (c_x - a_x)
//
// which is twice the signed area of the triangle (a, b, c).
//
// The sign of the result indicates the orientation:
//   > 0 : counterclockwise
//   < 0 : clockwise
//   = 0 : collinear
//
// This orientation test is important for rasterization. Suppose we
// want to test whether a point P lies inside the triangle (a, b, c).
// We form the three sub-triangles (P, b, c), (a, P, c), and (a, b, P).
// If P is inside, all of these orientations will have the same sign
// as the orientation of the original triangle. If P is outside, at
// least one orientation will differ.
//
// See also: Fabian Giesen,
// “The barycentric conspiracies”
// https://fgiesen.wordpress.com/2013/02/06/the-barycentric-conspirac/
#define _micro_draw_orient2D(a_x, a_y, b_x, b_y, c_x, c_y) \
    ( ((b_x) - (a_x)) * ((c_y) - (a_y)) - ((b_y) - (a_y)) * ((c_x) - (a_x)) )

#define _micro_draw_min(a, b) ((a) < (b) ? (a) : (b))
#define _micro_draw_max(a, b) ((a) > (b) ? (a) : (b))
#define _micro_draw_min3(a, b, c) (_micro_draw_min(_micro_draw_min((a), (b)), (c)))
#define _micro_draw_max3(a, b, c) (_micro_draw_max(_micro_draw_max((a), (b)), (c)))

// https://fgiesen.wordpress.com/2013/02/08/triangle-rasterization-in-practice/
MICRO_DRAW_DEF void
micro_draw_fill_triangle(unsigned char *data,
                         int data_width, int data_height,
                         int a_x, int a_y,
                         int b_x, int b_y,
                         int c_x, int c_y,
                         unsigned char *color, MicroDrawPixel pixel)
{
  // Compute triangle bounding box
  int minX = _micro_draw_min3(a_x, b_x, c_x);
  int minY = _micro_draw_min3(a_y, b_y, c_y);
  int maxX = _micro_draw_max3(a_x, b_x, c_x);
  int maxY = _micro_draw_max3(a_y, b_y, c_y);

  // Clip against screen bounds
  minX = _micro_draw_max(minX, 0);
  minY = _micro_draw_max(minY, 0);
  maxX = _micro_draw_min(maxX, data_width - 1);
  maxY = _micro_draw_min(maxY, data_height - 1);

  for (int row = minY; row <= maxY; ++row)
  {
    for (int col = minX; col <= maxX; ++col)
    {  
      // Determine barycentric coordinates
      int w0 = _micro_draw_orient2D(b_x, b_y, c_x, c_y, col, row);
      int w1 = _micro_draw_orient2D(c_x, c_y, a_x, a_y, col, row);
      int w2 = _micro_draw_orient2D(a_x, a_y, b_x, b_y, col, row);

      // If p is on or inside all edges, render pixel.
      if (w0 >= 0 && w1 >= 0 && w2 >= 0)
        micro_draw_pixel(data, data_width, data_height,
                         col, row, color, pixel);
    }
  }

  return;
}
  
MICRO_DRAW_DEF void
micro_draw_grid(unsigned char* data,
                int data_width, int data_height,
                int columns, int rows,
                unsigned char* color, MicroDrawPixel pixel)
{
  // Draw columns
  for (int x = 0; x < data_width; x += data_width / columns)
  {
    micro_draw_line(data, data_width, data_height,
                     x, 0, x, data_height,
                     color, pixel);
  }
  
  // Draw rows
  for (int y = 0; y < data_height; y += data_height / rows)
  {
    micro_draw_line(data, data_width, data_height,
                     0, y, data_width, y,
                     color, pixel);
  }
  return;
}

MICRO_DRAW_DEF void
micro_draw_get_color(unsigned char* data,
                     int data_width, int data_height,
                     int x, int y,
                     unsigned char** color, MicroDrawPixel pixel)
{
  if (x >= data_width || x < 0 || y >= data_height || y < 0) return;

  int channel_size = micro_draw_get_channel_size(pixel); // bytes
  size_t channels = micro_draw_get_channels(pixel);
  int index = (y * data_width + x) * channels * channel_size;

  *color = data + index;
  return;
}

MICRO_DRAW_DEF void
micro_draw_scaled(unsigned char* src_data,
                  int src_data_width, int src_data_height,
                  MicroDrawPixel src_pixel,
                  unsigned char* dest_data,
                  int dest_data_width, int dest_data_height,
                  MicroDrawPixel dest_pixel)
{
  for (int y = 0; y < dest_data_height; ++y)
  {
    for (int x = 0; x < dest_data_width; ++x)
    {
      int x_frame = (x * src_data_width) / (double)dest_data_width;
      int y_frame = (y * src_data_height) / (double)dest_data_height;
      unsigned char *color;
      micro_draw_get_color(src_data, src_data_width, src_data_height,
                           x_frame, y_frame, &color, src_pixel);
      micro_draw_pixel(dest_data, dest_data_width, dest_data_height,
                       x, y, color, dest_pixel);
    }
  }
  return;
}

MICRO_DRAW_DEF void
micro_draw_text(unsigned char* data,
                int data_width, int data_height,
                int text_x, int text_y, int text_size, char* text,
                unsigned char* color, MicroDrawPixel pixel)
{
  (void) data;
  (void) data_width;
  (void) data_height;
  (void) text_x;
  (void) text_y;
  (void) text_size;
  (void) text;
  (void) color;
  (void) pixel;
  
  assert(0 && "TODO");
  
  return;
}
  
#ifdef MICRO_DRAW_PPM

#include <stdio.h>

_Static_assert(_MICRO_DRAW_PIXEL_MAX == 1,
               "Updated MicroDrawPixel, should also update micro_draw_to_ppm");
MICRO_DRAW_DEF MicroDrawError
micro_draw_to_ppm(const char *filename,
                  unsigned char *data,
                  int data_width, int data_height,
                  MicroDrawPixel pixel)
{
  MicroDrawError error = MICRO_DRAW_OK;
  
  FILE* file = fopen(filename, "w+");
  if (file == NULL)
  {
    perror("Error opening file");
    return MICRO_DRAW_ERROR_OPEN_FILE;
  }

  int channels = micro_draw_get_channels(pixel);
  int channel_size = micro_draw_get_channel_size(pixel);
  int data_size = data_width * data_height * channels * channel_size;
  fprintf(file, "P6\n%d %d\n%d\n", data_width,
          data_height, (1 << (channel_size * 8)) - 1);

  switch(pixel)
  {
  case MICRO_DRAW_RGBA8:
    for (int i = 0; i < data_size; i += channels * channel_size)
    {
      fwrite(data + i, channel_size, (channels - 1), file);
    }
    break;
  default:
    goto done;
  }

 done:
  fclose(file);
  return error;
}

MICRO_DRAW_DEF MicroDrawError
micro_draw_from_ppm(const char* filename,
                    unsigned char **data,
                    int *data_width, int *data_height,
                    MicroDrawPixel *pixel)
{
  MicroDrawError error = MICRO_DRAW_OK;
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    perror("Error opening file");
    return MICRO_DRAW_ERROR_OPEN_FILE;
  }

  char magic[3];
  fread(magic, 1, 3, file);
  if (strcmp(magic, "P6\n") != 0)
  {
    error = MICRO_DRAW_ERROR_INVALID_MAGIC_NUMBER;
    goto done;
  }

  // TODO
  (void) data;
  (void) data_width;
  (void) data_height;
  (void) pixel;
  assert(0); // Not implemented
  
 done:
  fclose(file);
  return error;
}

unsigned char
micro_draw_font[128][MICRO_DRAW_FONT_HEIGHT][MICRO_DRAW_FONT_WIDTH] = {
  ['a'] = {
    { 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0 },
    { 0, 0, 0, 1, 0 },
    { 0, 1, 1, 1, 0 },
    { 1, 0, 0, 1, 0 },
    { 0, 1, 1, 1, 0 },
  },
};
  
#endif // MICRO_DRAW_PPM

#endif // MICRO_DRAW_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // MICRO_DRAW
