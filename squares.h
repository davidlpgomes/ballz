#ifndef _SQUARES_H_
#define _SQUARES_H_

#define S_COLS 8
#define S_M 10

#include "ballz.h"

typedef struct {
    int rows, columns;
    int side, margin;
    int offset;

    int* lifes;
} squares_t;

void free_squares(squares_t* squares);

void generate_square_line(game_t* g, squares_t* s, int rows_offset);

squares_t* init_squares();

void reset_squares(squares_t* squares);

int switch_squares_lines(squares_t* squares);

#endif
