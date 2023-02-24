#include <math.h>

#include "bouncers.h"
#include "display.h"
#include "helpers.h"

int are_all_bouncers_stopped(bouncers_t* bouncers) {
    bouncer_t* bouncer = bouncers->head;

    while (bouncer != NULL) {
        if (bouncer->dx || bouncer->dy) {
            return 0;
        }

        bouncer = bouncer->next;
    }

    return 1;
}

bouncer_t* create_bouncer(bouncers_t* bouncers) {
    bouncer_t* bouncer = malloc(sizeof(bouncer_t));
    must_init(bouncer, "bouncer");

    bouncer->x = 0;
    bouncer->y = 0;
    bouncer->dx = 0;
    bouncer->dy = 0;
    bouncer->shooted = 0;
    bouncer->previous = NULL;
    bouncer->next = NULL;

    if (bouncers->size) {
        bouncers->tail->next = bouncer;
        bouncer->previous = bouncers->tail;
    } else {
        bouncers->head = bouncer;
    }

    bouncers->size += 1;
    bouncers->tail = bouncer;

    return bouncer;
}

void free_bouncers(bouncers_t* bouncers) {
    if (bouncers->size) {
        bouncer_t* bouncer = bouncers->head;
        bouncer_t* next = bouncer->next;

        while (bouncer != NULL) {
            next = bouncer->next;
            free(bouncer);
            bouncer = next;
        }
    }

    free(bouncers);

    return;
}

void get_bouncer_row_and_column(bouncer_t* b, int l, int m, int* r, int* c) {
    *r = b->y / (l + m) - 1;
    *c = b->x / (l + m);
}

int has_a_bouncer_hit_bottom(bouncers_t* bouncers, int y_bottom) {
    if (!bouncers->size)
        return -1;

    bouncer_t* bouncer = bouncers->head;

    while (bouncer != NULL) {
        if (bouncer->shooted && bouncer->y == y_bottom)
            return 1;
        bouncer = bouncer->next;
    }

    return 0;
}

bouncers_t* init_bouncers() {
    bouncers_t* bouncers = malloc(sizeof(bouncers_t));
    must_init(bouncers, "bouncers");

    bouncers->head = NULL;
    bouncers->tail = NULL;
    bouncers->size = 0;

    bouncers->first_x = -1;
    bouncers->new_bouncers = 0;

    create_bouncer(bouncers);

    bouncers->last_shooted = bouncers->head;

    return bouncers;
}

bouncers_t* reset_bouncers(bouncers_t* bouncers, int x, int y) {
    free(bouncers);
    bouncers_t* b = init_bouncers();
    set_bouncers(b, x, y);

    return b;
}

void set_bouncer_speed_with_angle(bouncer_t* bouncer, float speed, float angle) {
    bouncer->dx = speed * cos(angle);
    bouncer->dy = speed * -sin(angle);
    return;
}

void set_bouncers(bouncers_t* bouncers, int x, int y) {
    bouncer_t* bouncer = bouncers->head;

    while (bouncer != NULL) {
        bouncer->x = x;
        bouncer->y = y;
        bouncer->dx = 0;
        bouncer->dy = 0;
        bouncer->shooted = 0;

        bouncer = bouncer->next;
    }

    return;
}

int test_bouncer_limits_collision(bouncer_t* b, bouncers_t* bouncers) {
    int col = 0;

    // LEFT
    if ((b->x - B_RADIUS) <= 0) {
        b->x = B_RADIUS;
        b->dx *= -1;
        col = 1;
    }

    // RIGHT
    if ((b->x + B_RADIUS) >= WIDTH) {
        b->x = WIDTH - B_RADIUS;
        b->dx *= -1;
        col = 1;
    }

    // TOP
    if ((b->y - B_RADIUS) <= Y_UP) {
        b->y = Y_UP + B_RADIUS;
        b->dy *= -1;
        col = 1;
    }

    // BOTTOM
    if (b->y > Y_BO) {
        b->y = Y_BO;
        b->dy = 0;
        b->dx = 0;

        if (bouncers->first_x == -1)
            bouncers->first_x = b->x;

        b->shooted = 1;
        col = 1;
    }

    return col;
}

int test_bouncer_squares_collision(bouncer_t* b, bouncers_t* bouncers, squares_t* squares) {
    int row, column;
    get_bouncer_row_and_column(b, squares->side, squares->margin, &row, &column);

    int r = squares->rows;
    int c = squares->columns;
    int* lifes = squares->lifes;

    int col = 0;

    for (int i = clamp(row - 2, 0, r); i < clamp(row + 2, 0, r); i++) {
        for (int j = clamp(column - 2, 0, c); j < clamp(column + 2, 0, c); j++) {
            if (!lifes[i * c + j])
                continue;

            int x1 = squares->side * j + squares->margin * (j + 1);
            int y1 = Y_UP + squares->side * (i - 1) + squares->margin * i;
            int x2 = x1 + squares->side;
            int y2 = y1 + squares->side;

            // TEST COLISION
            if (lifes[i * c + j] > 0) {
                if (
                    b->x + B_RADIUS >= x1 && b->x - B_RADIUS <= x2 &&
                    b->y + B_RADIUS >= y1 && b->y - B_RADIUS <= y2
                ) {
                    float nearestX = clamp(b->x, x1, x2);
                    float nearestY = clamp(b->y, y1, y2);

                    if (nearestX == x1) {
                        b->dx *= -1;
                        b->x = nearestX - B_RADIUS;
                    } else if (nearestX == x2) {
                        b->dx *= -1;
                        b->x = nearestX + B_RADIUS;
                    }

                    if (nearestY == y1) {
                        b->dy *= -1;
                        b->y = nearestY - B_RADIUS;
                    } else if (nearestY == y2) {
                        b->dy *= -1;
                        b->y = nearestY + B_RADIUS;
                    }

                    lifes[i * c + j] -= 1;
                    col = 1;
                }
            } else {
                if (get_distance(
                        b->x,
                        b->y,
                        x1 + squares->side / 2,
                        y1 + squares->side / 2
                    ) <= B_RADIUS + squares->side / 2
                ) {
                    lifes[i * c + j] = 0;
                    bouncers->new_bouncers += 1;
                }
            }
        }
    }

    return col;
}

int update_bouncers(bouncers_t* bouncers, squares_t* squares) {
    bouncer_t* b = bouncers->head;

    int col = 0;

    while (b != NULL) {
        b->x += b->dx;
        b->y += b->dy;

        if (b->y != Y_BO) {
            if (test_bouncer_limits_collision(b, bouncers))
                col = 1;
            
            if (test_bouncer_squares_collision(b, bouncers, squares))
                col = 1;
        }

        if (b->shooted && !b->dy) {
            if (b->x != bouncers->first_x && abs(b->x - bouncers->first_x) <= SPEED) {
                b->x = bouncers->first_x;
                b->dx = 0;
            } else if (b->x > bouncers->first_x)
                b->dx = -SPEED;
            else if (b->x < bouncers->first_x)
                b->dx = SPEED;
        }

        b = b->next;
    }

    return col;
}
