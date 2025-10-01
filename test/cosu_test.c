// SPDX-License-Identifier: MIT

#define MICRO_DRAW_IMPLEMENTATION
#include "../micro-draw.h"

#define RGFW_IMPLEMENTATION
#include "RGFW.h"

#include <time.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 800
#define SPEED 1
#define FPS 60
#define NOTE_FREQUENCY 6

typedef struct {
  unsigned int x;
  unsigned int y;
} Note;

struct NoteList;
typedef struct NoteList NoteList;

struct NoteList {
  Note note;
  NoteList *next;
};

typedef struct {
  NoteList *notes;
  RGFW_window* win;
  unsigned char *screen;
  RGFW_surface *surface;
  double delta_time;
  double note_time;
  double fps;
} Application;

void note_list_append(NoteList **list,
                      NoteList *new_note)
{
  if (!list || !new_note) return;

  new_note->next = NULL;
  if (!(*list))
  {
    *list = new_note;
    return;
  }
  
  NoteList *it = *list;
  while (it->next)
  {
    it = it->next;
  }

  it->next = new_note;
  return;
}

void note_list_remove(NoteList **list, NoteList *removed_note)
{
  if (!list || !*list) return;
  
  NoteList *prev = NULL;
  NoteList *it = *list;
  while (it)
  {
    if (it == removed_note)
    {
      if (prev)
      {
        prev->next = it->next;
      }
      else
      {
        *list = it->next;
      }
      free(it);
      break;
    }
    prev = it;
    it = it->next;
  }
  return;
}

void note_list_free(NoteList **list)
{
  if (!list || !*list) return;

  NoteList *it = *list;
  while (it)
  {
    NoteList *next = it->next;
    free(it);
    it = next;
  }

  *list = NULL;
  return;
}

int parse_osu_map(char *filename, NodeList **nodes)
{
  // TODO
  return 0;
}

#define MAGIC1 16843009  // a
#define MAGIC2 826366247 // c

// LCG pseudo random number generator
unsigned int lcg_seed = 69696969;
unsigned int lcg(const unsigned int seed)
{
  return (MAGIC1 * seed + MAGIC2);
}

void loop(void *arg)
{
  Application *app = (Application*) arg;
  unsigned char color_white[4] = {255, 255, 255, 255};
  unsigned char color_black[4] = {0, 0, 0, 255};
  unsigned char color_red[4] = {255, 0, 0, 255};
  
  RGFW_event event;
  clock_t frame_start = clock();

  while (RGFW_window_checkEvent(app->win, &event))
  {
    if (event.type == RGFW_quit)
      break;

    if (event.type == RGFW_keyPressed)
    {
      switch(event.key.sym)
      {
      case 'd':
        if (app->notes->note.x == 0
            && app->notes->note.y > 3 * (SCREEN_HEIGHT / 4))
        {
          note_list_remove(&app->notes, app->notes);
          printf("Note hit!\n");
        }
        break;
      case 'f':
        if (app->notes->note.x == 1 * (SCREEN_WIDTH / 4)
            && app->notes->note.y > 3 * (SCREEN_HEIGHT / 4))
        {
          note_list_remove(&app->notes, app->notes);
          printf("Note hit!\n");
        }
        break;
      case 'j':
        if (app->notes->note.x == 2 * (SCREEN_WIDTH / 4)
            && app->notes->note.y > 3 * (SCREEN_HEIGHT / 4))
        {
          note_list_remove(&app->notes, app->notes);
          printf("Note hit!\n");
        }
        break;
      case 'k':
        if (app->notes->note.x == 3 * (SCREEN_WIDTH / 4)
            && app->notes->note.y > 3 * (SCREEN_HEIGHT / 4))
        {
          note_list_remove(&app->notes, app->notes);
          printf("Note hit!\n");
        }
        break;
      default:
        break;
      }
    }
  }

  if (app->note_time > 1 / (double) NOTE_FREQUENCY)
  {
    app->note_time = 0;

    lcg_seed = lcg(lcg_seed);
    if (lcg_seed % 5 != 4) // skip a note, also lcg does not work modulo 4
    {
      NoteList *note = malloc(sizeof(NoteList));
      *note = (NoteList) {
        .note = (Note) {
          .x = (lcg_seed % 5) * (SCREEN_WIDTH / 4),
          .y = 0,
        },
        .next = NULL,
      };
      note_list_append(&app->notes, note);
      printf("Created note at position x: %u, y: %u\n", note->note.x, note->note.y);
    }
  }
  
  #ifndef __EMSCRIPTEN__
  if (app->delta_time > 1 / app->fps)
  {
  #endif
    // Draw
    app->delta_time = 0;
      
    micro_draw_clear(app->screen, SCREEN_WIDTH, SCREEN_HEIGHT,
                     color_black, MICRO_DRAW_RGBA8); 

    NoteList *it = app->notes;
    while(it)
    {
      if (it->note.y > SCREEN_HEIGHT)
      {
        // Lost note
        NoteList *next = it->next;
        note_list_remove(&app->notes, it);
        printf("removed note\n");
        it = next;
        continue;
      }

      // Hit line
      micro_draw_line(app->screen,
                      SCREEN_WIDTH, SCREEN_HEIGHT,
                      0, 3 * (SCREEN_HEIGHT / 4) + SCREEN_HEIGHT / 10,
                      SCREEN_WIDTH, 3 * (SCREEN_HEIGHT / 4) + SCREEN_HEIGHT / 10,
                      color_red, MICRO_DRAW_RGBA8);


      micro_draw_fill_rect(app->screen,
                           SCREEN_WIDTH, SCREEN_HEIGHT,
                           it->note.x, it->note.y, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 10,
                           color_white, MICRO_DRAW_RGBA8);
      it->note.y += 1.0 / app->fps * SCREEN_HEIGHT * SPEED;

      it = it->next;
    }
    
    RGFW_window_blitSurface(app->win, app->surface);
    
  #ifndef __EMSCRIPTEN__
   }
  #endif

  clock_t frame_end = clock();
  double diff = (double)(frame_end - frame_start) / CLOCKS_PER_SEC;
  app->delta_time += diff;
  app->note_time += diff;
}

int main(void)
{
  Application app;

  app.notes = NULL;
  app.win = RGFW_createWindow("cosu",
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
  app.note_time = 0;
  app.fps = FPS;

  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(loop, &app, app.fps, 1);
  #else
  while (RGFW_window_shouldClose(app.win) == RGFW_FALSE) loop(&app);
  #endif // __EMSCRIPTEN__

  note_list_free(&app.notes);
  RGFW_surface_free(app.surface);
  free(app.screen);
  RGFW_window_close(app.win);
  
  return 0;
}
