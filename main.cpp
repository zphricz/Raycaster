#include "Screen.h"
#include "Game.h"
#include <iostream>

static void error(char * name) {
    printf("Usage: %s [Screen_x Screen_y]\n", name);
    exit(1);
}

int main(int argc, char* argv[])
{
    bool default_screen;
    int screen_width;
    int screen_height;
    SDL_DisplayMode display;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        exit(1);
    }
    SDL_GetCurrentDisplayMode(0, &display);
    atexit(SDL_Quit);

    switch (argc) {
    case 1:
        default_screen = true;
        break;
    case 3:
        screen_width = atoi(argv[1]);
        screen_height = atoi(argv[2]);
        default_screen = false;
        if (screen_width <= 0 || screen_height <= 0) {
            error(argv[0]);
        }
        break;
    default:
        error(argv[0]);
    }

    if (default_screen) {
        screen_width = display.w;
        screen_height = display.h;
    }

    bool full_screen;
    if (screen_width == display.w && screen_height == display.h) {
        full_screen = true;
    } else {
        full_screen = false;
    }

    Screen scr(screen_width, screen_height, full_screen, "Raycaster", true, true);
    Game g(&scr);
    g.run();
    return 0;
}
