#include <math.h>
#include <stdio.h>

#include "helpers.h"

float clamp(float x, float a, float b) {
    return max(a, min(x, b));
}

int is_mouse_on_button(int x, int y, int x_c, int y_c, int x_p, int y_p) {
    if (
        x >= (x_c - x_p) && x <= (x_c + x_p) &&
        y >= (y_c - y_p) && y <= (y_c + y_p)
    )
        return 1;

    return 0;
}

float get_distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

void get_rad_angle_and_distance(
    float* angle, float* distance, int x1, int y1, int x2, int y2
) {
    *distance = get_distance(x1, y1, x2, y2);
    float c = (x1 - x2) / *distance;
    *angle = acos(c);

    return;
}

float max(float a, float b) {
    if (a > b)
        return a;
    
    return b;
}

float min(float a, float b) {
    if (a < b)
        return a;

    return b;
}

void must_init(bool test, const char *description) {
    if (test)
        return;

    printf("Nao foi possivel inicializar %s\n", description);
    exit(EXIT_FAILURE);

    return;
}

int randint(int n, int m) {
    return rand() % (m - n + 1) + n;
}
