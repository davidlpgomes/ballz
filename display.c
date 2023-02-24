#include <stdio.h>

#include "display.h"
#include "helpers.h"

void free_bitmaps(bitmaps_t* bitmaps) {
    al_destroy_bitmap(bitmaps->arrow_left_black);
    al_destroy_bitmap(bitmaps->arrow_left_white);
    al_destroy_bitmap(bitmaps->bg);
    al_destroy_bitmap(bitmaps->icon);

    free(bitmaps);

    return;
}

void free_fonts(fonts_t* fonts) {
    al_destroy_font(fonts->small);
    al_destroy_font(fonts->medium);
    al_destroy_font(fonts->big);

    free(fonts);

    return;
}

void free_help(help_t* help) {
    free(help);

    return;
}

void free_menu(menu_t* menu) {
    free(menu);

    return;
}

void free_sounds(sounds_t* sounds) {
    al_destroy_sample(sounds->collision);
    al_destroy_sample(sounds->easter);
    al_destroy_sample(sounds->game_over);

    free(sounds);

    return;
}

void free_window(window_t* window) {
    al_destroy_display(window->display);
    al_destroy_timer(window->timer);
    al_destroy_event_queue(window->queue);

    free(window);

    return;
}

void init_allegro() {
    must_init(al_init(), "allegro");
    must_init(al_init_primitives_addon(), "primitivos");
    must_init(al_install_keyboard(), "teclado");
    must_init(al_install_mouse(), "mouse");
    must_init(al_init_font_addon(), "font");
    must_init(al_init_ttf_addon(), "ttf");
    must_init(al_init_image_addon(), "image");
    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "codec");
    must_init(al_reserve_samples(16), "reserve samples");

    return;
}

bitmaps_t* init_bitmaps() {
    bitmaps_t* bitmaps = malloc(sizeof(bitmaps_t));
    must_init(bitmaps, "bitmaps");

    char path[STR_SIZE];

    sprintf(path, "%s/resources/images/arrow_left_b.png", al_get_current_directory());
    bitmaps->arrow_left_black = al_load_bitmap(path);
    must_init(bitmaps->arrow_left_black, "arrow left black");

    sprintf(path, "%s/resources/images/arrow_left_w.png", al_get_current_directory());
    bitmaps->arrow_left_white = al_load_bitmap(path);
    must_init(bitmaps->arrow_left_white, "arrow left white");

    sprintf(path, "%s/resources/images/background.png", al_get_current_directory());
    bitmaps->bg = al_load_bitmap(path);
    must_init(bitmaps->bg, "background");

    sprintf(path, "%s/resources/images/icon.png", al_get_current_directory());
    bitmaps->icon = al_load_bitmap(path);
    must_init(bitmaps->icon, "icon");

    return bitmaps;
}

fonts_t* init_fonts() {
    fonts_t* fonts = malloc(sizeof(fonts_t));
    must_init(fonts, "fontes");

    char path[1024];
    sprintf(path, "%s/resources/fonts/JosefinSans.ttf", al_get_current_directory());

    ALLEGRO_FONT* small = al_load_ttf_font(path, 22, 0);
    must_init(small, "fonte");

    ALLEGRO_FONT* medium = al_load_ttf_font(path, 28, 0);
    must_init(medium, "fonte");

    ALLEGRO_FONT* big = al_load_ttf_font(path, 56, 0);
    must_init(big, "fonte");

    fonts->small = small;
    fonts->medium = medium;
    fonts->big = big;

    return fonts;
}

help_t* init_help() {
    help_t* help = malloc(sizeof(help_t));
    must_init(help, "help");

    help->x1 = WIDTH / 2 - WIDTH / 4;
    help->y1 = HEIGHT / 2 - HEIGHT / 4;
    help->x2 = WIDTH / 2 + WIDTH / 4;
    help->y2 = HEIGHT / 2 + HEIGHT / 4;

    help->x_ok = WIDTH / 2;
    help->y_ok = help->y2 - 50;

    help->x_btn_padding = 70;
    help->y_btn_padding = 28;

    return help;
}

menu_t* init_menu() {
    menu_t* menu = malloc(sizeof(menu_t));
    must_init(menu, "menu");

    menu->x_title = WIDTH / 2;
    menu->y_title = HEIGHT / 6;

    menu->x_play = WIDTH / 2;
    menu->y_play = HEIGHT / 2.5;

    menu->x_scores = WIDTH / 2;
    menu->y_scores = HEIGHT / 2;

    menu->x_exit = WIDTH / 2;
    menu->y_exit = HEIGHT / 1.7;

    menu->x_btn_padding = 70;
    menu->y_btn_padding = 25;

    return menu;
}

sounds_t* init_sounds() {
    sounds_t* sounds = malloc(sizeof(sounds_t));
    must_init(sounds, "sounds");

    char path[STR_SIZE];

    sprintf(path, "%s/resources/sounds/col.wav", al_get_current_directory());
    sounds->collision = al_load_sample(path);
    must_init(sounds->collision, "collision sound");

    sprintf(path, "%s/resources/sounds/augh.ogg", al_get_current_directory());
    sounds->easter = al_load_sample(path);
    must_init(sounds->easter, "easter sound");

    sprintf(path, "%s/resources/sounds/ugh.ogg", al_get_current_directory());
    sounds->game_over = al_load_sample(path);
    must_init(sounds->game_over, "game over sound");

    return sounds;
}

window_t* init_window() {
    window_t* window = malloc(sizeof(window_t));
    must_init(window, "window");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FRAMERATE);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "fila");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_DEPTH_SIZE, 16, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* display = al_create_display(WIDTH, HEIGHT);
    must_init(display, "display");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    al_set_window_title(display, "Ballz");

    window->timer = timer;
    window->queue = queue;
    window->display = display;

    return window;
}

void play_sample(ALLEGRO_SAMPLE* sample) {
    al_play_sample(
        sample,
        1.0,
        0.0,
        1.1,
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );

    return;
}

void set_cursor(window_t* window, int hover) {
    if (hover)
        al_set_system_mouse_cursor(
            window->display,
            ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK
        );
    else
        al_set_system_mouse_cursor(
            window->display,
            ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT
        );

    return;
}
