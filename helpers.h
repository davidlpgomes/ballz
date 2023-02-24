#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <allegro5/allegro.h>

#define STR_SIZE 1024

float clamp(float x, float a, float b);

int is_mouse_on_button(int x, int y, int x_c, int y_c, int x_p, int y_p);

float get_distance(int x1, int y1, int x2, int y2);

void get_rad_angle_and_distance(float* angle, float* distance, int x1, int y1, int x2, int y2);

float max(float a, float b);

float min(float a, float b);

void must_init(bool test, const char *description);

int randint(int n, int m);

#endif
