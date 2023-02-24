#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define PIXEL(r,g,b) (al_map_rgb((unsigned char) r, (unsigned char) g, (unsigned char) b))
#define BPIXEL(pixel) PIXEL(pixel.R, pixel.G, pixel.B)

#define FRAMERATE 60

#define WIDTH 600
#define HEIGHT 800

#define Y_BO (HEIGHT * 0.8)
#define Y_UP (HEIGHT * 0.1)

typedef struct {
  ALLEGRO_EVENT_QUEUE* queue;
  ALLEGRO_TIMER* timer;
  ALLEGRO_DISPLAY* display;
} window_t;

typedef struct {
  ALLEGRO_FONT* small;
  ALLEGRO_FONT* medium;
  ALLEGRO_FONT* big;
} fonts_t;

typedef struct {
  ALLEGRO_BITMAP* arrow_left_black;
  ALLEGRO_BITMAP* arrow_left_white;
  ALLEGRO_BITMAP* bg;
  ALLEGRO_BITMAP* icon;
} bitmaps_t;

typedef struct {
  ALLEGRO_SAMPLE* easter;
  ALLEGRO_SAMPLE* game_over;
  ALLEGRO_SAMPLE* collision;
} sounds_t;

typedef struct {
  int x_title;
  int y_title;

  int x_play;
  int y_play;

  int x_scores;
  int y_scores;

  int x_exit;
  int y_exit;

  int x_btn_padding;
  int y_btn_padding;
} menu_t;

typedef struct {
  int x1, y1;
  int x2, y2;

  int x_ok;
  int y_ok;

  int x_btn_padding;
  int y_btn_padding;
} help_t;

void free_bitmaps(bitmaps_t* bitmaps);

void free_fonts(fonts_t* fonts);

void free_help(help_t* help);

void free_menu(menu_t* menu);

void free_sounds(sounds_t* sounds);

void free_window(window_t* window);

void init_allegro();

bitmaps_t* init_bitmaps();

fonts_t* init_fonts();

help_t* init_help();

menu_t* init_menu();

sounds_t* init_sounds();

window_t* init_window();

void play_sample(ALLEGRO_SAMPLE* sample);

void set_cursor(window_t* window, int hover);

#define WHITE       PIXEL(255,255,255)
#define BLACK       PIXEL(0,0,0)

#define BACKGROUND  PIXEL(106, 103, 206)
#define BOUNCER     PIXEL(238, 243, 210)
#define SECONDARY   PIXEL(148, 126, 195)

#define SQUARE_1    PIXEL(120, 88, 166)
#define SQUARE_2    PIXEL(91, 75, 138)
#define SQUARE_3    PIXEL(76, 53, 117)
#define SQUARE_4    PIXEL(55, 27, 88)

#endif
