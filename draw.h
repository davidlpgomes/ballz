#ifndef _DRAW_H_
#define _DRAW_H_

#include "display.h"
#include "helpers.h"
#include "bouncers.h"
#include "score.h"
#include "squares.h"

#define HELP_STRS "H/F1 - Alterna ajuda,F - Alterna FPS,P - Alterna pausa,R - Recomeça o jogo,ESC - Volta ao menu,Autor: David Gomes\0"

void draw(
    game_t* g, window_t* w, fonts_t* f, mouse_t* m, menu_t* mn, 
    help_t* h, bitmaps_t* bm, squares_t* s, bouncers_t* b, scores_t* sc
);

// RETURN 1 IF MOUSE IS HOVERING BUTTON, ELSE 0
int draw_arrow(bitmaps_t* bitmaps, int x, int y);

void draw_bitmap(ALLEGRO_BITMAP* background, int x, int y);

void draw_bouncer_pointer(bouncers_t* bouncers, float angle, float distance);

void draw_bouncers(bouncers_t* bouncers);

void draw_bouncers_counter(bouncers_t* bouncers, fonts_t* fonts);

void draw_fps(int fps, fonts_t* fonts);

// RETURN 1 IF MOUSE IS HOVERING BUTTON, ELSE 0
int draw_help(help_t* help, fonts_t* fonts, int x, int y);

void draw_limits(fonts_t* fonts, int score, int record, int cheat);

// RETURN 1 IF MOUSE IS HOVERING BUTTON, ELSE 0
int draw_menu(menu_t* menu, fonts_t* fonts, int x, int y);

void draw_scores(scores_t* scores, fonts_t* fonts, int x, int y);

void draw_squares(window_t* window, squares_t* squares, fonts_t* fonts);

#endif
