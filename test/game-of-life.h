//////////////////////////////////////////////////////////////////////
// SPDX-License-Identifier: MIT
//
// game-of-life.h
// --------------
//
// Conway's Game of Life logic implemented as a C99 header-only
// library.
//
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// License: MIT
//
//
// Usage
// -----
//
// Do this:
//
//   #define GAME_OF_LIFE_IMPLEMENTATION
//
// before you include this file in *one* C or C++ file to create the
// implementation.
//
// i.e. it should look like this:
//
//   #include ...
//   #include ...
//   #include ...
//   #define GAME_OF_LIFE_IMPLEMENTATION
//   #include "game-of-life.h"
//
// If you want basic printing functionality from this library, you
// need to #define GAME_OF_LIFE_PRINT
//
// To run a simultation, simply call `game_of_life_simulate`.
//
// Check out the `example.c` file for a complete example.
//

#ifndef _GAME_OF_LIFE_H_
#define _GAME_OF_LIFE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

//
// Function Declarations
//

// Count the number of alive neighbours of a point
int game_of_life_count_neighbours(bool *universe,
                                  int width, int height,
                                  int x, int y);

// Calculate next state
//
// At each step in time, the following transitions occur:
//
//  - Any live cell with fewer than two live neighbours dies, as if by
//    underpopulation.
//  - Any live cell with two or three live neighbours lives on to the
//    next generation.
//  - Any live cell with more than three live neighbours dies, as if
//    by overpopulation.
//  - Any dead cell with exactly three live neighbours becomes a live
//    cell, as if by reproduction.
//
// Args:
//  - universe: pointer to the universe, which is an array of boolean
//    cells of size width * height
//  - scratch: a temporary buffer with the same dimention as [universe],
//    used to compute the next state
//  - width: the width of [universe]
//  - height: the height of [universe]
void game_of_life_simulate(bool **universe, bool **scratch,
                           int width, int height);

#ifdef GAME_OF_LIFE_PRINT

// Print the universe with `0` (dead) or `1` (alive) char cells
// Note: this includes the stdio header.
void game_of_life_print(bool *universe, int width, int height);

#endif // GAME_OF_LIFE_PRINT

//
// Implementation
//

#ifdef GAME_OF_LIFE_IMPLEMENTATION

int game_of_life_count_neighbours(bool *universe,
                                  int width, int height,
                                  int x, int y)
{
  int count = 0;
  for (int offset_x = -1; offset_x <= 1; ++offset_x)
    for (int offset_y = -1; offset_y <= 1; ++offset_y)
    {
      int new_x = x + offset_x;
      int new_y = y + offset_y;

      if (new_x < 0 || new_y < 0 ||
          new_x >= width || new_y >= height ||
          (offset_x == 0 && offset_y == 0))
        continue;

      if (universe[new_y * width + new_x])
        count++;
    }

  return count;
}

void game_of_life_simulate(bool **universe, bool **scratch,
                           int width, int height)
{
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
    {
      int neighbours_count =
        game_of_life_count_neighbours(*universe, width, height, x, y);

      if ((*universe)[y * width + x])
      {
        (*scratch)[y * width + x] =
          (neighbours_count == 2 || neighbours_count == 3);
      }
      else
      {
        (*scratch)[y * width + x] = (neighbours_count == 3);
      }
    }

  bool *tmp = *universe;
  *universe = *scratch;
  *scratch = tmp;

  return;
}

#ifdef GAME_OF_LIFE_PRINT

#include <stdio.h>

void game_of_life_print(bool *universe, int width, int height)
{
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      if (universe[y * width + x])
      {
        printf("1 ");
      }
      else
      {
        printf("0 ");
      }
    }
    printf("\n");
  }
}

#endif // GAME_OF_LIFE_PRINT

#endif // GAME_OF_LIFE_IMPLEMENTATION

#ifdef __cplusplus
}
#endif
  
#endif // _GAME_OF_LIFE_H_
