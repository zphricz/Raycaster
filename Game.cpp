#include <iostream>
#include <cmath>
#include <thread>
#include <future>
#include <vector>
#include "Game.h"

using namespace std;

static const float PI = 3.14159265358979323846264338327950288419716939937510;
static const float TWO_PI = 2.0 * PI;

static float rad(float theta) {
    return theta * PI / 180.0;
}

static int num_threads;
const static float turn_rate = rad(6.0); // degrees/frame
const static float move_rate = 0.1; // distance/frame
const static float fov = rad(75.0); // degrees
static float plane_distance;
static float plane_width;
const static int map_width = 20;
const static int map_height = 20;
const static float wall_size = 1.0;
static int map[map_height][map_width] = {
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 2},
    {2, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 3, 3, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 2, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 2},
    {2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
    {2, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 2, 0, 0, 1, 0, 0, 1, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 2},
    {2, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 3, 0, 0, 1, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0, 2},
    {2, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
};

void Game::handle_input() {
    static bool w_pressed = false;
    static bool a_pressed = false;
    static bool s_pressed = false;
    static bool d_pressed = false;
    static bool left_pressed = false;
    static bool right_pressed = false;
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
            case SDLK_LEFT: {
                left_pressed = true;
                break;
            }
            case SDLK_RIGHT: {
                right_pressed = true;
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
            case SDLK_LEFT: {
                left_pressed = false;
                break;
            }
            case SDLK_RIGHT: {
                right_pressed = false;
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
    Vec2 last_position = position;
    Vec2 direction_moved{0, 0};
    if (w_pressed) {
        direction_moved += {cos(direction), sin(direction)};
    }
    if (a_pressed) {
        direction_moved += {cos(direction + rad(270.0)), sin(direction + rad(270.0))};
    }
    if (s_pressed) {
        direction_moved += {cos(direction + rad(180.0)), sin(direction + rad(180.0))};
    }
    if (d_pressed) {
        direction_moved += {cos(direction + rad(90.0)), sin(direction + rad(90.0))};
    }
    if (left_pressed) {
        direction -= turn_rate;
        if (direction < 0.0) {
            direction += TWO_PI;
        }
    }
    if (right_pressed) {
        direction += turn_rate;
        if (direction >= TWO_PI) {
            direction -= TWO_PI;
        }
    }
    position += direction_moved * move_rate;
    if (map[(int)position.y][int(position.x)]) {
        if ((int)position.y > (int)last_position.y) {
            // collided on y
            position = last_position;
            position.x += direction_moved.x * move_rate;
        } else {
            // collided on x
            position = last_position;
            position.y += direction_moved.y * move_rate;
        }
        if (map[(int)position.y][int(position.x)]) {
            position = last_position;
        }
    }
}

Game::Game(Screen* scr) : 
    scr(scr),
    position(2.0, 2.0),
    direction(rad(45.0)),
    running(true) {
    scr->set_recording_style("images", 5);
    plane_width = scr->width;
    plane_distance = plane_width / (tan(fov / 2.0) * 2.0);
    num_threads = (int)thread::hardware_concurrency();
}

Game::~Game() {
}

void Game::process_slice(Game& self, int slice) {
    for (int i = slice * self.scr->width / num_threads; i < (slice + 1) * self.scr->width / num_threads; ++i) {
        bool y_hit;
        float distance;
        float plane_dist_x = i - self.scr->width / 2 + 0.5;
        float angle = self.direction + atan(plane_dist_x / plane_distance);
        Vec2 dist_next;
        Vec2 ray_orientation(cos(angle), sin(angle));
        bool y_positive = (ray_orientation.y > 0.0) ? true: false;
        bool x_positive = (ray_orientation.x > 0.0) ? true: false;
        Vec2 diff_dist(abs(1.0 / ray_orientation.x), abs(1.0 / ray_orientation.y));
        int x = (int)self.position.x;
        int y = (int)self.position.y;
        int dx;
        int dy;
        if (x_positive) {
            dx = 1;
            dist_next.x = (floor(self.position.x) + 1.0 - self.position.x) * diff_dist.x;
        } else {
            dx = -1;
            dist_next.x = -(floor(self.position.x) - self.position.x) * diff_dist.x;
        }
        if (y_positive) {
            dy = 1;
            dist_next.y = (floor(self.position.y) + 1.0 - self.position.y) * diff_dist.y;
        } else {
            dy = -1;
            dist_next.y = -(floor(self.position.y) - self.position.y) * diff_dist.y;
        }

        while (true) {
            if (dist_next.x < dist_next.y) {
                // Check the next interesection on an x boundary
                x += dx;
                if (map[y][x]) {
                    y_hit = false;
                    break;
                }
                dist_next.x += diff_dist.x;
            } else {
                // Check the next interesection on a y boundary
                y += dy;
                if (map[y][x]) {
                    y_hit = true;
                    break;
                }
                dist_next.y += diff_dist.y;
            }
        }
        Color c{0, 0, 0};
        switch(map[y][x]) {
        case RED:
            c.r = 255;
            break;
        case GREEN:
            c.g = 255;
            break;
        case BLUE:
            c.b = 255;
            break;
        default:
            c.r = 0;
            c.g = 0;
            c.b = 0;
            break;
        }
        if (y_hit) {
            c /= 2;
            distance = dist_next.y;
        } else {
            distance = dist_next.x;
        }
        distance *= cos(angle - self.direction); // Correct the fish-eye effect
        int length = rint(wall_size * plane_distance / distance);
        self.scr->ver_line(i, (self.scr->height - length) / 2, (self.scr->height + length) / 2, c);
    }
}

void Game::draw_game() {
    // Draw the ceiling as gray (floors stay black)
    scr->fill_rect(0, 0, scr->width - 1, scr->height / 2, {75, 75, 75}); // Ceiling
    scr->fill_rect(0, scr->height / 2 + 1, scr->width - 1, scr->height - 1, {25, 25, 25}); // Floor

    // Perform ray casting in parallel
    vector<future<void>> futures;
    futures.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
        futures.push_back(async(launch::async, process_slice, ref(*this), i));
    }
    for (auto& f: futures) {
        f.get();
    }
}

void Game::run() {
    while (running) {
        handle_input();
        draw_game();
        scr->commit();
    }
}

