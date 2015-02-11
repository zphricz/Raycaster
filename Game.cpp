#include <iostream>
#include <cmath>
#include <future>
#include <vector>
#include <fstream>
#include "Game.h"

using namespace std;

static const float PI = 3.14159265358979323846264338327950288419716939937510;
static const float TWO_PI = 2.0 * PI;

static float rad(float theta) {
    return theta * PI / 180.0;
}

static float deg(float theta) {
    return theta * 180.0 / PI;
}

static const float turn_rate = rad(180.0); // degrees/sec
static const float move_rate = 5.0; // distance/sec
static const float wall_size = 1.0;

char& Game::map_at(int x, int y) {
    return map[map_width * y + x];
}

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
            case SDLK_1: {
                fov = rad(deg(fov) - 1.0);
                if (fov < rad(10.0)) {
                    fov = rad(deg(fov) + 1.0);
                }
                plane_distance = plane_width / (tan(fov / 2.0) * 2.0);
                break;
            }
            case SDLK_2: {
                fov = rad(deg(fov) + 1.0);
                if (fov >= PI) {
                    fov = rad(deg(fov) - 1.0);
                }
                plane_distance = plane_width / (tan(fov / 2.0) * 2.0);
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
        direction -= turn_rate * scr->frame_time();
        if (direction < 0.0) {
            direction += TWO_PI;
        }
    }
    if (right_pressed) {
        direction += turn_rate * scr->frame_time();
        if (direction >= TWO_PI) {
            direction -= TWO_PI;
        }
    }
    if (direction_moved.magnitude() > 0.0) {
        direction_moved.normalize();
    }
    position += direction_moved * move_rate * scr->frame_time();
    int old_x = (int)last_position.x;
    int old_y = (int)last_position.y;
    int new_x = (int)position.x;
    int new_y = (int)position.y;
    if (map_at(new_x, new_y)) { // Collision found
        if (new_y != old_y && new_x == old_x) {
            // Crossed y bound and not x bound
            position.y = last_position.y;
        } else if (new_y == old_y && new_x != old_x) {
            // Crossed x bound and not y bound
            position.x = last_position.x;
        } else if (new_x != old_x && new_y != old_y) {
            // Crossed both bounds
            if (!map_at(old_x, new_y)) {
                position.x = last_position.x;
            } else if (!map_at(new_x, old_y)) {
                position.y = last_position.y;
            } else {
                position = last_position;
            }
        } else {
            // We should never get here
        }
    }
}

Game::Game(Screen* scr, const char* map_name, int num_threads) : 
    scr(scr),
    fov(rad(90.0)),
    plane_width(scr->width),
    plane_distance(scr->width / (tan(fov / 2.0) * 2.0)),
    num_threads(num_threads),
    running(true) {
    scr->set_recording_style("images", 5);

    // Parse the map
    ifstream f(map_name);
    if (!f.is_open()) {
        cout << "Error opening: " << map_name << endl;
        exit(1);
    }
    f >> map_width >> map_height;
    f >> position.x >> position.y >> direction;
    direction = rad(direction);
    map.reserve(map_width * map_height);
    char max_val = 0;
    while (f.peek() == '\n') { f.ignore(); }
    for (int y = 0; y < map_height; ++y) {
        char index;
        for (int x = 0;; ++x) {
            f.get(index);
            if (index == '\n') {
                break;
            }
            if (x >= map_width) {
                cout << "ERROR: Read in too many cells in x direction" << endl;
                exit(1);
            }
            if (index == '\0') {
                cout << "ERROR: '\0' is  a reserved grid value" << endl;
                exit(1);
            }
            if (index == ' ') {
                index = '\0';
            }
            max_val = max(max_val, index);
            map_at(x, y) = index;
        }
    }
    while (f.peek() == '\n') { f.ignore(); }
    int r, g, b;
    f >> r >> g >> b;
    ceiling_color = {(Uint8)r, (Uint8)g, (Uint8)b};
    f >> r >> g >> b;
    floor_color = {(Uint8)r, (Uint8)g, (Uint8)b};

    while (f.peek() == '\n') { f.ignore(); }
    colors.reserve(max_val + 1);
    while (!f.eof()) {
        char index;
        f >> index >> r >> g >> b;
        if (index > max_val) {
            continue;
        }
        colors[index] = {(Uint8)r, (Uint8)g, (Uint8)b};
    }
}

Game::~Game() {
}

void Game::process_slice(Game& self, int slice) {
    for (int i = slice * self.scr->width / self.num_threads; i < (slice + 1) * self.scr->width / self.num_threads; ++i) {
        bool y_hit;
        float distance;
        float plane_dist_x = i - self.plane_width / 2.0 + 0.5;
        float angle = self.direction + atan(plane_dist_x / self.plane_distance);
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
                if (self.map_at(x, y)) {
                    y_hit = false;
                    break;
                }
                dist_next.x += diff_dist.x;
            } else {
                // Check the next interesection on a y boundary
                y += dy;
                if (self.map_at(x, y)) {
                    y_hit = true;
                    break;
                }
                dist_next.y += diff_dist.y;
            }
        }
        Color c = self.colors[self.map_at(x, y)];
        if (y_hit) {
            c /= 2;
            distance = dist_next.y;
        } else {
            distance = dist_next.x;
        }
        distance *= cos(angle - self.direction); // Correct the fish-eye effect
        int length = rint(wall_size * self.plane_distance / distance);
        self.scr->ver_line(i, (self.scr->height - length) / 2, (self.scr->height + length) / 2, c);
    }
}

void Game::draw_game() {
    // Draw the ceiling as gray (floors stay black)
    scr->fill_rect(0, 0, scr->width - 1, scr->height / 2, ceiling_color);
    scr->fill_rect(0, scr->height / 2 + 1, scr->width - 1, scr->height - 1, floor_color);

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
        static int i = 0;
        if(i++ == 10) {
            cout << "FPS: " << scr->fps() << endl;
            i = 0;
        }
    }
}

