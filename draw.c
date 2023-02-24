#include <stdio.h>
#include <math.h>

#include "draw.h"

void draw(
    game_t* g, window_t* w, fonts_t* f, mouse_t* m, menu_t* mn, 
    help_t* h, bitmaps_t* bm, squares_t* s, bouncers_t* b, scores_t* sc
) {
    al_clear_to_color(WHITE);
    draw_bitmap(bm->bg, 0, 0);

    if (g->general_state == MENU) {
        set_cursor(w, draw_menu(mn, f, m->x, m->y));
        return;
    }

    if (g->general_state == SCORES) {
        set_cursor(w, draw_arrow(bm, m->x, m->y));
        draw_scores(sc, f, m->x, m->y);
        return;
    }

    draw_squares(w, s, f);
    draw_bouncers(b);
    draw_limits(f, g->score, sc->scores[0], g->cheat);

    if (g->game_state != SHOOTING && g->game_state != RUNNING)
        draw_bouncers_counter(b, f);
    
    if (g->game_state == MOUSE_CLICKED)
        draw_bouncer_pointer(b, m->angle, m->distance);

    if (g->general_state == HELP) {
        set_cursor(w, draw_help(h, f, m->x, m->y));
        draw_arrow(bm, m->x, m->y);
        return;
    }

    set_cursor(w, draw_arrow(bm, m->x, m->y));

    return;
}

int draw_arrow(bitmaps_t* bitmaps, int x, int y) {
    if (is_mouse_on_button(x, y, 20, 20, 40, 40)) {
        al_draw_bitmap(bitmaps->arrow_left_white, 20, 20, 0);
        return 1;
    }

    al_draw_bitmap(bitmaps->arrow_left_black, 20, 20, 0);
    return 0;
}

void draw_bitmap(ALLEGRO_BITMAP* bitmap, int x, int y) {
    al_draw_bitmap(bitmap, x, y, 0);
    return;
}

void draw_bouncer_pointer(bouncers_t* bouncers, float angle, float distance) {
    float distance_factor = distance / 100;

    distance_factor = clamp(distance_factor, 1.4, 3.4);

    for (int i = 0; i < 16; i++) {
        int a_x = bouncers->head->x + 10 * (i + 1) * distance_factor * cos(angle);
        int a_y = bouncers->head->y + 10 * (i + 1) * distance_factor * -sin(angle);
        al_draw_filled_circle(a_x, a_y, 5, WHITE);
    }

    return;
}

void draw_bouncers(bouncers_t* bouncers) {
    bouncer_t* bouncer = bouncers->head;

    while (bouncer != NULL) {
        al_draw_filled_circle(
            bouncer->x,
            bouncer->y,
            B_RADIUS,
            B_COLOR
        );

        bouncer = bouncer->next;
    }

    return;
}

void draw_bouncers_counter(bouncers_t* bouncers, fonts_t* fonts) {
    char s_num[1024];
    sprintf(s_num, "x%d", bouncers->size);

    al_draw_text(
        fonts->small,
        WHITE,
        bouncers->head->x - 10,
        bouncers->head->y - 3 * B_RADIUS,
        ALLEGRO_ALIGN_CENTER,
        s_num
    );
}

void draw_fps(int fps, fonts_t* fonts) {
    char s_fps[10];

    sprintf(s_fps, "FPS: %d", fps);

    al_draw_text(
        fonts->small,
        BLACK,
        WIDTH / 2,
        0,
        ALLEGRO_ALIGN_CENTER,
        s_fps
    );
    
    return;
}

int draw_help(help_t* help, fonts_t* fonts, int x, int y) {
    al_draw_filled_rectangle(
        help->x1,
        help->y1,
        help->x2,
        help->y2,
        SQUARE_3
    );

    char line[1024] = HELP_STRS;
    char* text = strtok(line, ",");
    int i = 1;
    while (text != NULL) {
        al_draw_text(
            fonts->small,
            WHITE,
            WIDTH / 3,
            help->y1 + 50 * i,
            ALLEGRO_ALIGN_LEFT,
            text
        );

        text = strtok(NULL, ",");
        i++;
    }

    ALLEGRO_COLOR color = BLACK;
    int hover = 0;

    if (is_mouse_on_button(
        x,
        y,
        help->x_ok,
        help->y_ok,
        help->x_btn_padding,
        help->y_btn_padding
    )) {
        color = WHITE;
        hover = 1;
    }

     al_draw_text(
        fonts->medium,
        color,
        help->x_ok,
        help->y_ok,
        ALLEGRO_ALIGN_CENTER,
        "OK"
    );

    return hover;
}

void draw_limits(fonts_t* fonts, int score, int record, int cheat) {
    al_draw_filled_rectangle(
        0,
        Y_BO + B_RADIUS,
        WIDTH,
        HEIGHT,
        SECONDARY
    );

    al_draw_filled_rectangle(
        0,
        0,
        WIDTH,
        Y_UP,
        SECONDARY
    );

    char s_score[1024];
    sprintf(s_score, "%d", score);

    al_draw_text(
        fonts->big,
        BLACK,
        WIDTH / 2,
        Y_UP / 4,
        ALLEGRO_ALIGN_CENTER,
        s_score
    );

    if (record) {
        al_draw_text(
            fonts->small,
            BLACK,
            WIDTH - 100,
            Y_UP / 4,
            ALLEGRO_ALIGN_CENTER,
            "RECORDE"
        );

        sprintf(s_score, "%d", record);

        al_draw_text(
            fonts->small,
            BLACK,
            WIDTH - 100,
            Y_UP / 4 + 25,
            ALLEGRO_ALIGN_CENTER,
            s_score
        );
    }

    if (cheat)
        al_draw_text(
            fonts->small,
            BLACK,
            WIDTH / 2,
            HEIGHT - 50,
            ALLEGRO_ALIGN_CENTER,
            "MODO CHEAT"
        );

    return;
}

int draw_menu(menu_t* menu, fonts_t* fonts, int x, int y) {
    al_draw_text(
        fonts->big,
        BLACK,
        menu->x_title,
        menu->y_title,
        ALLEGRO_ALIGN_CENTER,
        "Ballz"
    );

    ALLEGRO_COLOR color = BLACK;
    int hover = 0;

    if (is_mouse_on_button(
        x,
        y,
        menu->x_play,
        menu->y_play,
        menu->x_btn_padding,
        menu->y_btn_padding
    )) {
        color = WHITE;
        hover = 1;
    }

     al_draw_text(
        fonts->medium,
        color,
        menu->x_play,
        menu->y_play,
        ALLEGRO_ALIGN_CENTER,
        "JOGAR"
    );

    color = BLACK;

    if (is_mouse_on_button(
        x,
        y,
        menu->x_scores,
        menu->y_scores,
        menu->x_btn_padding,
        menu->y_btn_padding
    )) {
        color = WHITE;
        hover = 1;
    }

    al_draw_text(
        fonts->medium,
        color,
        menu->x_scores,
        menu->y_scores,
        ALLEGRO_ALIGN_CENTER,
        "PONTUAÇÕES"
    );

    color = BLACK;

    if (is_mouse_on_button(
        x,
        y,
        menu->x_exit,
        menu->y_exit,
        menu->x_btn_padding,
        menu->y_btn_padding
    )) {
        color = WHITE;
        hover = 1;
    }

    al_draw_text(
        fonts->medium,
        color,
        menu->x_exit,
        menu->y_exit,
        ALLEGRO_ALIGN_CENTER,
        "SAIR"
    );

    return hover;
}

void draw_scores(scores_t* scores, fonts_t* fonts, int x, int y) {
    al_draw_text(
        fonts->medium,
        BLACK,
        WIDTH / 2,
        HEIGHT / 8,
        ALLEGRO_ALIGN_CENTER,
        "PONTUAÇÕES"
    );

    if (scores->size == 0) {
        al_draw_text(
            fonts->small,
            WHITE,
            WIDTH / 2,
            HEIGHT / 2,
            ALLEGRO_ALIGN_CENTER,
            "Ainda não há uma pontuação"
        );
        return;
    }

    char s_sc[STR_SIZE];
    int h = HEIGHT / 4;

    for (int i = 0; i < scores->size; i++) {
        sprintf(s_sc, "%dº", i + 1);
        al_draw_text(
            fonts->small,
            BLACK,
            WIDTH / 3,
            h,
            ALLEGRO_ALIGN_CENTER,
            s_sc
        );

        sprintf(s_sc, "%d", scores->scores[i]);
        al_draw_text(
            fonts->small,
            WHITE,
            WIDTH / 2,
            h,
            ALLEGRO_ALIGN_CENTER,
            s_sc
        );

        h += 50;
    }
}

void draw_squares(window_t* window, squares_t* squares, fonts_t* fonts) {
    int x, y, life;
    char s_num[1024];
    ALLEGRO_COLOR rect_color;

    for (int i = 0; i < squares->rows; i++) {
        for (int j = 0; j < squares->columns; j++) {
            if (!squares->lifes[i * squares->columns + j])
                continue;

            life = squares->lifes[i * squares->columns + j];

            x = squares->side * j + squares->margin * (j + 1);
            y = squares->offset + Y_UP + squares->side * (i - 1) + squares->margin * i;

            if (y < Y_UP && !squares->offset)
                continue;

            if (life > 0) {
                if (life < 10)
                    rect_color = SQUARE_1;
                else if (life < 20)
                    rect_color = SQUARE_2;
                else if (life < 30)
                    rect_color = SQUARE_3;
                else
                    rect_color = SQUARE_4;

                al_draw_filled_rectangle(
                    x,
                    y,
                    x + squares->side,
                    y + squares->side,
                    rect_color
                );

                if (y + squares->side / 2 > Y_UP) {
                    sprintf(s_num, "%d", life);
                    al_draw_text(
                        fonts->small,
                        BLACK,
                        x + squares->side / 2,
                        y + squares->side / 2,
                        ALLEGRO_ALIGN_CENTER,
                        s_num
                    );
                }
            } else {
                al_draw_filled_circle(
                    x + squares->side / 2,
                    y + squares->side / 2,
                    squares->side / 6,
                    WHITE
                );

                al_draw_circle(
                    x + squares->side / 2,
                    y + squares->side / 2,
                    (float) squares->side /
                    ((float) (al_get_timer_count(window->timer) % 20) / 20 + 3),
                    WHITE,
                    3
                );
            }
        }
    }

    return;
}
