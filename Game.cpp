#include <iostream>
#include <cmath>
#include "Game.h"

using namespace std;

static const float PI = 3.14159265358979323846264338327950288419716939937510;
static const float TWO_PI = 2.0 * PI;

static float rad(float theta) {
    return theta * PI / 180.0;
}

const static float turn_rate = rad(6.0); // degrees/frame
const static float move_rate = 0.1; // distance/frame
const static float fov = rad(60.0); // degrees
static float plane_distance;
const static int map_width = 20;
const static int map_height = 20;
const static float wall_size = 1.0;
static int map[map_height][map_width] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void Game::handle_input() {
    static bool w_pressed = false;
    static bool a_pressed = false;
    static bool s_pressed = false;
    static bool d_pressed = false;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT: {
            running = false;
            break;
        }
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE: {
                running = false;
                break;
            }
            case SDLK_SPACE: {
                break;
            }
            case SDLK_BACKSPACE: {
                break;
            }
            case 'w': {
                w_pressed = true;
                break;
            }
            case 'a': {
                a_pressed = true;
                break;
            }
            case 's': {
                s_pressed = true;
                break;
            }
            case 'd': {
                d_pressed = true;
                break;
            }
            default: {
                break;
            }
            }
            break;
        }
        case SDL_KEYUP: {
            switch (event.key.keysym.sym) {
            case 'w': {
                w_pressed = false;
                break;
            }
            case 'a': {
                a_pressed = false;
                break;
            }
            case 's': {
                s_pressed = false;
                break;
            }
            case 'd': {
                d_pressed = false;
                break;
            }
            default: {
                break;
            }
            }
        }
        default: {
             break;
        }
        }
    }
    if (w_pressed) {
        Vec2 orientation(cos(direction), sin(direction));
        position += orientation * move_rate;
    }
    if (a_pressed) {
        direction -= turn_rate;
        if (direction < 0.0) {
            direction += TWO_PI;
        }
    }
    if (s_pressed) {
        Vec2 orientation(cos(direction), sin(direction));
        position -= orientation * move_rate;
    }
    if (d_pressed) {
        direction += turn_rate;
        if (direction >= TWO_PI) {
            direction -= TWO_PI;
        }
    }
}

Game::Game(Screen* scr) : 
    scr(scr),
    position(2.0, 2.0),
    direction(rad(45.0)),
    running(true) {
    scr->set_recording_style("images", 5);
    plane_distance = scr->width / tan(fov / 2.0);
}

Game::~Game() {
}

void Game::draw_game() {
    scr->cls();
    // Draw the ceiling as gray (floors stay black)
    for (int y = 0; y < scr->height / 2; ++y) {
        scr->hor_line(y, 0, scr->width - 1, {50, 50, 50});
    }

    // Perform ray casting
    float ray_increment = fov / scr->width;
    float angle = direction - fov / 2.0;
    for (int i = 0; i < scr->width; ++i, angle += ray_increment) {
        Vec2 ray_orientation(cos(angle), sin(angle));
        Vec2 ray(0.0, 0.0);
        float x_diff;
        float y_diff;
        Vec2 intersection;
        Color c{0, 255, 0};

        if (abs(sin(angle)) > abs(cos(angle))) {
            x_diff = 1.0 / tan(angle);
            if (sin(angle) > 0.0) {
                y_diff = 1.0;
                intersection.y = floor(position.y) + 1.0;
            } else {
                y_diff = -1.0;
                intersection.y = floor(position.y);
            }
            c.g += 105;
            intersection.x = position.x + (intersection.y - position.y) / tan(angle);
            if (x_diff < 0.0) {
                while (!map[lrint(intersection.y - 1.0)][(int)floor(intersection.x - 1.0)]) {
                    intersection.y += y_diff;
                    intersection.x += x_diff;
                }
            } else {
                while (!map[lrint(intersection.y - 1.0)][(int)ceil(intersection.x - 1.0)]) {
                    intersection.y += y_diff;
                    intersection.x += x_diff;
                }
            }
        } else {
            y_diff = tan(angle);
            if (cos(angle) > 0.0) {
                x_diff = 1.0;
                intersection.x = floor(position.x) + 1.0;
            } else {
                x_diff = -1.0;
                intersection.x = floor(position.x);
            }
            intersection.y = position.y + (intersection.x - position.x) * tan(angle);
            if (y_diff < 0.0) {
                while (!map[(int)floor(intersection.y - 1.0)][lrint(intersection.x - 1.0)]) {
                    intersection.y += y_diff;
                    intersection.x += x_diff;
                }
            } else {
                while (!map[(int)ceil(intersection.y - 1.0)][lrint(intersection.x - 1.0)]) {
                    intersection.y += y_diff;
                    intersection.x += x_diff;
                }
            }
        }
        float distance = intersection.magnitude() * cos(angle - direction);
        int length = lrint(wall_size * plane_distance / distance);
        scr->ver_line(i, (scr->height - length) / 2, (scr->height + length) / 2, c);
    }
}

void Game::run() {
    while (running) {
        handle_input();
        draw_game();
        scr->commit();
        static int i = 0;
        if (i++ == 30) {
            i = 0;
            cout << "(" << position.x << ", " << position.y << "): " << direction << endl;
        }
    }
}

