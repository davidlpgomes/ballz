#ifndef _BOUNCERS_H_
#define _BOUNCERS_H_

#define B_RADIUS 12
#define B_COLOR BOUNCER
#define SPEED 12

#include "squares.h"

struct bouncer {
    float x, y;
    float dx, dy;
    int shooted;

    struct bouncer* previous;
    struct bouncer* next;
};
typedef struct bouncer bouncer_t;

struct bouncers {
    bouncer_t* head;
    bouncer_t* tail;
    int size;

    bouncer_t* last_shooted;

    float first_x;
    int new_bouncers;
};
typedef struct bouncers bouncers_t;

int are_all_bouncers_stopped(bouncers_t* bouncers);

bouncer_t* create_bouncer(bouncers_t* bouncers);

void free_bouncers(bouncers_t* bouncers);

void get_bouncer_row_and_column(bouncer_t* b, int l, int m, int* r, int* c);

int has_a_bouncer_hit_bottom(bouncers_t* bouncers, int y_bottom);

bouncers_t* init_bouncers();

bouncers_t* reset_bouncers(bouncers_t* bouncers, int x, int y);

void set_bouncer_speed_with_angle(bouncer_t* bouncer, float speed, float angle);

void set_bouncers(bouncers_t* bouncers, int x, int y);

int test_bouncer_limits_collision(bouncer_t* b, bouncers_t* bouncers);

int test_bouncer_squares_collision(bouncer_t* b, bouncers_t* bouncers, squares_t* squares);

// RETURN 1 IF COLLISSION, ELSE 0
int update_bouncers(bouncers_t* bouncers, squares_t* squares);

#endif
