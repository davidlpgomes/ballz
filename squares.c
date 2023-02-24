#include "display.h"
#include "helpers.h"
#include "squares.h"

void free_squares(squares_t* squares) {
    free(squares->lifes);
    free(squares);

    return;
}

void generate_square_line(game_t* g, squares_t* s, int rows_offset) {
    int c = s->columns;
    float prob;

    for (int j = 0; j < c; j++) {
        prob = 40;

        if (s->lifes[(rows_offset + 1) * c + c])
            prob = 25;

        if (prob >= randint(0, 100)) {
            if (70 >= randint(0, 100))
                s->lifes[rows_offset * c + j] = g->score;
            else
                s->lifes[rows_offset * c + j] = (g->score) * 2;
            
            if (g->cheat)
                s->lifes[rows_offset * c + j] = 1;
        }
        else
            s->lifes[rows_offset * c + j] = 0;
    }

    if (g->score > 1)
        s->lifes[rows_offset * c + randint(0, c)] = -1;
}

squares_t* init_squares() {
    squares_t* squares = malloc(sizeof(squares_t));
    must_init(squares, "squares");

    squares->margin = S_M;
    squares->side = (int) WIDTH / S_COLS - S_M - S_M / S_COLS;

    squares->columns = S_COLS;
    squares->rows = (int) (Y_BO - Y_UP) / (squares->side + S_M) + 2;

    squares->lifes = calloc(squares->rows * S_COLS, sizeof(int));

    squares->offset = 0;

    return squares;
}

void reset_squares(squares_t* squares) {
    for (int i = 0; i < squares->rows; i++)
        for (int j = 0; j < squares->columns; j++)
            squares->lifes[i * squares->columns + j] = 0;

    return;
}

int switch_squares_lines(squares_t* squares) {
    int r = squares->rows;
    int c = squares->columns;
    int* lifes = squares->lifes;

    for (int i = r - 1; i > 0; i--) {
        for (int j = 0; j < c; j++)
            lifes[i * c + j] = lifes[(i - 1) * c + j];
    }

    for (int j = 0; j < c; j++)
        if (lifes[(r - 1) * c + j] > 0)
            return 1;
        else if (lifes[(r - 1) * c + j] == -1)
            lifes[(r - 1) * c + j] = 0;

    return 0;
}
