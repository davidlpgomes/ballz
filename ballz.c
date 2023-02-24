#include "ballz.h"
#include "bouncers.h"
#include "display.h"
#include "draw.h"
#include "helpers.h"
#include "score.h"
#include "squares.h"

void go_back(game_t* game) {
    if (game->general_state == MENU)
        game->general_state = EXIT;
    else
        game->general_state = MENU;
}

void set_general_state(window_t* window, game_t* game, int state) {
    game->general_state = state;
    set_cursor(window, 0);

    return;
}

void check_go_back_button(game_t* game, mouse_t* mouse) {
    if (is_mouse_on_button(
        mouse->x,
        mouse->y,
        20,
        20,
        40,
        40
    ))
        go_back(game);
}

void check_menu_buttons(window_t* w, game_t* g, menu_t* m, mouse_t* mo, sounds_t* s) {
    if (is_mouse_on_button(
        mo->x,
        mo->y,
        m->x_play,
        m->y_play,
        m->x_btn_padding,
        m->y_btn_padding
    ))
        set_general_state(w, g, GAME);
    else if (is_mouse_on_button(
        mo->x,
        mo->y,
        m->x_scores,
        m->y_scores,
        m->x_btn_padding,
        m->y_btn_padding
    ))
        set_general_state(w, g, SCORES);
    else if (is_mouse_on_button(
        mo->x,
        mo->y,
        m->x_exit,
        m->y_exit,
        m->x_btn_padding,
        m->y_btn_padding
    ))
        set_general_state(w, g, EXIT);

    if (mo->x > WIDTH - 50 && mo->y > HEIGHT - 50)
        play_sample(s->easter);

    return;
}

void check_help_buttons(window_t* w, game_t* g, help_t* h, mouse_t* m) {
    if (is_mouse_on_button(
        m->x,
        m->y,
        h->x_ok,
        h->y_ok,
        h->x_btn_padding,
        h->y_btn_padding
    ))
        set_general_state(w, g, GAME);

    return;
}

mouse_t* init_mouse() {
    mouse_t* mouse = malloc(sizeof(mouse_t));
    must_init(mouse, "mouse");

    mouse->x = -1;
    mouse->y = -1;

    mouse->start_x = -1;
    mouse->start_y = -1;

    mouse->angle = -1;
    mouse->distance = -1;

    return mouse;
}

game_t* init_game() {
    game_t* game = malloc(sizeof(game_t));
    must_init(game, "game");

    game->general_state = MENU;
    game->game_state = WAITING_CLICK;

    game->redraw = 1;
    game->score = 1;
    game->cheat = 0;

    return game;
}

fps_t* init_fps() {
    fps_t* fps = malloc(sizeof(fps));
    must_init(fps, "fps");

    fps->avg = 1;
    fps->show = 0;
    fps->time = al_get_time();

    return fps;
}

void update_fps(fps_t* fps, fonts_t* fonts) {
    double new_time = al_get_time();
    fps->avg = (int) 1 / (new_time - fps->time);
    fps->time = new_time;

    if (fps->show)
        draw_fps(fps->avg, fonts);
}

void keyboard_pressed(int key, game_t* game, fps_t* fps) {
    if (key == ALLEGRO_KEY_C) {
        game->cheat = !game->cheat;
        return;
    }

    if (key == ALLEGRO_KEY_P) {
        if (game->general_state == GAME)
            game->general_state = PAUSE;
        else if (game->general_state == PAUSE)
            game->general_state = GAME;

        return;
    }
    
    if (key == ALLEGRO_KEY_H || key == ALLEGRO_KEY_F1) {
        if (game->general_state == GAME)
            game->general_state = HELP;
        else if (game->general_state == HELP)
            game->general_state = GAME;

        return;
    }

    if (key == ALLEGRO_KEY_ESCAPE) {
        go_back(game);
        return;
    }

    if (key == ALLEGRO_KEY_R) {
        if (game->general_state == GAME)
            game->game_state = GAME_OVER;

        return;
    }

    if (key == ALLEGRO_KEY_F)
        fps->show = !fps->show;

    return;
}

int main() {
    srand(time(0));

    init_allegro();
    window_t* window = init_window();
    fonts_t* fonts = init_fonts();
    help_t* help = init_help();
    menu_t* menu = init_menu();
    bitmaps_t* bitmaps = init_bitmaps();
    sounds_t* sounds = init_sounds();

    al_set_display_icon(window->display, bitmaps->icon);

    fps_t* fps = init_fps();
    game_t* game = init_game();
    mouse_t* mouse = init_mouse();
    scores_t* scores = init_scores();

    bouncers_t* bouncers = init_bouncers();
    set_bouncers(bouncers, WIDTH / 2, Y_BO);

    squares_t* squares = init_squares(S_COLS, S_M);
    generate_square_line(game, squares, 2);

    ALLEGRO_EVENT event;
    al_start_timer(window->timer);
    double begin = al_get_time();

    for (;;) {
        al_wait_for_event(window->queue, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_TIMER:
                if (game->general_state == EXIT)
                    break;

                if (game->general_state == GAME)
                    if (update_bouncers(bouncers, squares))
                        play_sample(sounds->collision);

                game->redraw = 1;
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                mouse->x = event.mouse.x;
                mouse->y = event.mouse.y;
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                if (game->general_state == MENU) {
                    check_menu_buttons(window, game, menu, mouse, sounds);
                    break;
                } else if (game->general_state == HELP) {
                    check_help_buttons(window, game, help, mouse);
                    break;
                }

                check_go_back_button(game, mouse);

                if (
                    game->general_state == GAME &&
                    game->game_state == WAITING_CLICK
                ) {
                    game->game_state = MOUSE_CLICKED;
                    mouse->start_x = event.mouse.x;
                    mouse->start_y = event.mouse.y;
                }
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                if (
                    game->general_state == GAME &&
                    game->game_state == MOUSE_CLICKED &&
                    mouse->angle > 0
                )
                    game->game_state = SHOOTING;

                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                keyboard_pressed(event.keyboard.keycode, game, fps);
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                game->general_state = EXIT;
                break;
        }

        if (game->general_state == EXIT)
            break;

        if (!game->redraw || !al_is_event_queue_empty(window->queue))
            continue;

        game->redraw = 0;

        draw(
            game, window, fonts, mouse, menu, help,
            bitmaps, squares, bouncers, scores
        );
        update_fps(fps, fonts);

        if (game->general_state != GAME) {
            al_flip_display();
            continue;
        }

        switch (game->game_state) {
            case RUNNING:
                if (!are_all_bouncers_stopped(bouncers))
                    break;

                for (int i = 0; i < bouncers->new_bouncers; i++)
                    create_bouncer(bouncers);

                set_bouncers(bouncers, bouncers->first_x, Y_BO);
                bouncers->new_bouncers = 0;
                bouncers->first_x = -1;

                game->game_state = CHANGING_ROUND;
                game->score += 1;
                generate_square_line(game, squares, 1);

                break;
            case CHANGING_ROUND:
                squares->offset += 5;

                if (squares->offset >= squares->side + squares->margin) {
                    squares->offset = 0;

                    if (!switch_squares_lines(squares)) {
                        game->game_state = WAITING_CLICK;
                    } else
                        game->game_state = GAME_OVER;
                }
                break;
            case MOUSE_CLICKED:
                get_rad_angle_and_distance(
                    &mouse->angle, &mouse->distance, 
                    mouse->start_x, mouse->start_y, mouse->x, mouse->y
                );
                break;
            case SHOOTING:
                if (al_get_time() - begin < .1)
                    break;

                begin = al_get_time();
                set_bouncer_speed_with_angle(
                    bouncers->last_shooted,
                    SPEED,
                    mouse->angle
                );
                bouncers->last_shooted->shooted = 1;

                bouncers->last_shooted = bouncers->last_shooted->next;
                if (bouncers->last_shooted == NULL) {
                    bouncers->last_shooted = bouncers->head;
                    game->game_state = RUNNING;
                }

                break;
            case GAME_OVER:
                play_sample(sounds->game_over);
                save_score(scores, game->score);
                game->score = 1;
                mouse->angle = 0;

                bouncers = reset_bouncers(bouncers, WIDTH / 2, HEIGHT * 0.8);
                reset_squares(squares);
                generate_square_line(game, squares, 2);

                game->game_state = WAITING_CLICK;
                break;
        }

        al_flip_display();
    }

    free_bouncers(bouncers);
    free_squares(squares);
    free_scores(scores);
    free(game);
    free(fps);
    free(mouse);

    free_bitmaps(bitmaps);
    free_fonts(fonts);
    free_help(help);
    free_sounds(sounds);
    free_menu(menu);
    free_window(window);

    return 0;
}
