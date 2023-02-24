#ifndef _BALLZ_H_
#define _BALLZ_H_

typedef struct {
    int x, y;
    int start_x, start_y;

    float angle;
    float distance;
} mouse_t;

typedef struct {
    int general_state;
    int game_state;

    int redraw;
    int score;
    int cheat;
} game_t;

typedef struct {
    int avg;
    int show;
    double time;
} fps_t;

enum game_enum {
    WAITING_CLICK,
    MOUSE_CLICKED,
    SHOOTING,
    RUNNING,
    CHANGING_ROUND,
    GAME_OVER
};

enum general_enum {
    MENU,
    GAME,
    PAUSE,
    HELP,
    SCORES,
    EXIT
};

#endif
