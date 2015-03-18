#include <iostream>
#include <cmath>
#include <future>
#include <fstream>
#include "Game.h"

using namespace std;
using namespace Linear;

static const float PI = M_PI;
static const float TWO_PI = M_PI * 2;

static float rad(float theta) { return theta * PI / 180.0; }

static float clip_angle(float angle) {
  if (angle < 0.0) {
    return angle + TWO_PI;
  }
  if (angle >= TWO_PI) {
    return angle - TWO_PI;
  }
  return angle;
}

static const float height_rate = 0.5;      // units/sec
static const float turn_rate = rad(180.0); // degrees/sec
static const float move_rate = 5.0;        // distance/sec
static const float fov_rate = rad(90.0);   // degrees/sec
static const float wall_size = 1.0;

char &Game::map_at(int x, int y) { return map[map_width * y + x]; }

void Game::handle_input() {
  static const Uint8 *key_state = SDL_GetKeyboardState(NULL);
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        running = false;
        break;
      default:
        break;
      }
      break;
    case SDL_MOUSEMOTION: {
      direction += event.motion.xrel * rad(0.75);
      direction = clip_angle(direction);
      pitch -= event.motion.yrel * rad(0.23);
      pitch = max<float>(rad(-89.0), min<float>(pitch, rad(89.0)));
      break;
    }
    default:
      break;
    }
  }

  Vec2f last_position = position;
  Vec2f direction_moved(0.0, 0.0);
  if (key_state[SDL_SCANCODE_UP]) {
    height += height_rate * scr->frame_time();
    height = min<float>(height, 1.0);
  }
  if (key_state[SDL_SCANCODE_DOWN]) {
    height -= height_rate * scr->frame_time();
    height = max<float>(0.0, height);
  }
  if (key_state[SDL_SCANCODE_LEFT]) {
    direction -= turn_rate * scr->frame_time();
    direction = clip_angle(direction);
  }
  if (key_state[SDL_SCANCODE_RIGHT]) {
    direction += turn_rate * scr->frame_time();
    direction = clip_angle(direction);
  }
  if (key_state[SDL_SCANCODE_1]) {
    fov = fov - fov_rate * scr->frame_time();
    fov = max<float>(rad(10.0), fov);
    plane_distance = plane_width / (tan(fov / 2.0) * 2.0);
  }
  if (key_state[SDL_SCANCODE_2]) {
    fov = fov + fov_rate * scr->frame_time();
    fov = min<float>(fov, rad(179.0));
    plane_distance = plane_width / (tan(fov / 2.0) * 2.0);
  }
  if (key_state[SDL_SCANCODE_W]) {
    direction_moved += Vec2f(direction);
  }
  if (key_state[SDL_SCANCODE_A]) {
    direction_moved += Vec2f(direction + rad(270.0));
  }
  if (key_state[SDL_SCANCODE_S]) {
    direction_moved += Vec2f(direction + rad(180.0));
  }
  if (key_state[SDL_SCANCODE_D]) {
    direction_moved += Vec2f(direction + rad(90.0));
  }
  if (direction_moved.magnitude() > 0.0) {
    direction_moved.normalize();
  }
  position += direction_moved * move_rate * scr->frame_time();
  int old_x = (int)last_position.x;
  int old_y = (int)last_position.y;
  int new_x = (int)position.x;
  int new_y = (int)position.y;
  if (map_at(old_x, new_y) && map_at(new_x, old_y) &&
      !map_at(new_x, new_y)) { // We just phased past a corner
    position = last_position;
  } else if (map_at(new_x, new_y)) { // Collision found
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

Game::Game(SoftScreen *scr, const char *map_name, int num_threads)
    : scr(scr), height(0.5), pitch(rad(0.0)), fov(rad(90.0)),
      plane_width(scr->width), plane_height(scr->height),
      plane_distance(scr->width / (tan(fov / 2.0) * 2.0)),
      num_threads(num_threads), running(true) {
  scr->set_recording_style("images", 5);

  // Parse the map
  ifstream f(map_name);
  if (!f.is_open()) {
    cout << "Error opening: " << map_name << endl;
    exit(1);
  }
  while (f.peek() == '\n') {
    f.ignore();
  }
  vector<vector<char>> temp_map;
  char max_val = 0;
  map_width = 0;
  map_height = 0;
  while (f.peek() != '\n') {
    map_height++;
    vector<char> line;
    int width = 0;
    while (f.peek() != '\n') {
      width++;
      char index;
      f.get(index);
      if (index == '\0') {
        cout << "ERROR: '\0' is  a resized grid value" << endl;
        exit(1);
      }
      if (index == ' ') {
        index = '\0';
      }
      max_val = max<char>(max_val, index);
      line.push_back(index);
    }
    f.ignore();
    temp_map.push_back(move(line));
    map_width = max<int>(width, map_width);
  }
  map.resize(map_width * map_height);
  for (int y = 0; y < map_height; ++y) {
    for (int x = 0; x < temp_map[y].size(); ++x) {
      map_at(x, y) = temp_map[y][x];
    }
  }

  while (f.peek() == '\n') {
    f.ignore();
  }
  f >> position.x >> position.y >> direction;
  direction = rad(direction);

  while (f.peek() == '\n') {
    f.ignore();
  }
  int r, g, b, a;
  f >> r >> g >> b;
  ceiling_color = {(Uint8)r, (Uint8)g, (Uint8)b, 255};
  f >> r >> g >> b;
  floor_color = {(Uint8)r, (Uint8)g, (Uint8)b, 255};

  while (f.peek() == '\n') {
    f.ignore();
  }
  colors.resize(max_val + 1);
  colors['\0'] = {0, 0, 0, 0};
  while (!f.eof()) {
    char index;
    f >> index >> r >> g >> b >> a;
    if (index > max_val) {
      cout << "Ignoring color for index: " << index;
    }
    colors[index] = {(Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a};
  }
  SDL_SetRelativeMouseMode(SDL_TRUE);
}

Game::~Game() {}

void Game::render_slice(int slice) {
  float pitch_offset = tan(pitch) * plane_width;
  for (int i = slice * scr->width / num_threads;
       i < (slice + 1) * scr->width / num_threads; ++i) {
    bool y_hit;
    float distance;
    float plane_dist_x = i - plane_width / 2.0 + 0.5;
    float angle = direction + atan(plane_dist_x / plane_distance);
    float dist_next_x;
    float dist_next_y;
    Vec2f ray_orientation(angle);
    bool y_positive = (ray_orientation.y > 0.0) ? true : false;
    bool x_positive = (ray_orientation.x > 0.0) ? true : false;
    float diff_dist_x = abs(1.0 / ray_orientation.x);
    float diff_dist_y = abs(1.0 / ray_orientation.y);
    int x = (int)position.x;
    int y = (int)position.y;
    int dx;
    int dy;
    if (x_positive) {
      dx = 1;
      dist_next_x = (floor(position.x) + 1.0 - position.x) * diff_dist_x;
    } else {
      dx = -1;
      dist_next_x = -(floor(position.x) - position.x) * diff_dist_x;
    }
    if (y_positive) {
      dy = 1;
      dist_next_y = (floor(position.y) + 1.0 - position.y) * diff_dist_y;
    } else {
      dy = -1;
      dist_next_y = -(floor(position.y) - position.y) * diff_dist_y;
    }

    char last_block = '\0';
    char next_block;
    SDL_Color c{0, 0, 0, 0};
    while (true) {
      if (dist_next_x < dist_next_y) {
        // Check the next interesection on an x boundary
        x += dx;
        y_hit = false;
        distance = dist_next_x;
        dist_next_x += diff_dist_x;
      } else {
        // Check the next interesection on a y boundary
        y += dy;
        y_hit = true;
        distance = dist_next_y;
        dist_next_y += diff_dist_y;
      }
      next_block = map_at(x, y);
      if (next_block != last_block) {
        // Draw blocks at all locations where two different blocks meet
        SDL_Color new_color;
        if (next_block == '\0') {
          new_color = colors[last_block];
        } else {
          new_color = colors[next_block];
        }
        if (y_hit) {
          c = blend(c, new_color / 2);
        } else {
          c = blend(c, new_color);
        }
        // Correct the fish-eye effect
        distance *= cos(angle - direction);
        float height_offset = (height - 0.5) * plane_distance / distance;
        float offset = height_offset + pitch_offset;
        float length = wall_size * plane_distance / distance;
        if (c.a == 255) {
          // Draw totally opaque line and quit
          scr->ver_line(i, rint((plane_height - length) / 2.0 + offset),
                        rint((plane_height + length) / 2.0 + offset), c);
          break;
        } else {
          // Draw transparent line and continue raycasting
          scr->ver_line(i, rint((plane_height - length) / 2.0 + offset),
                        rint(plane_height / 2.0 + pitch_offset),
                        blend(c, ceiling_color));
          scr->ver_line(i, rint(plane_height / 2.0 + pitch_offset) + 1,
                        rint((plane_height + length) / 2.0 + offset),
                        blend(c, floor_color));
        }
        last_block = next_block;
      }
    }
  }
}

void Game::draw_game() {
  // Draw the ceiling as gray (floors stay black)
  float pitch_offset = tan(pitch) * plane_width;
  scr->fill_rect(0, 0, scr->width - 1, rint(plane_height / 2.0 + pitch_offset),
                 ceiling_color);
  scr->fill_rect(0, rint(plane_height / 2.0 + pitch_offset) + 1, scr->width - 1,
                 scr->height - 1, floor_color);

  if (num_threads == 1) {
    render_slice(0);
  } else {
    // Perform ray casting in parallel
    vector<future<void>> futures;
    futures.reserve(num_threads);
    for (int i = 0; i < num_threads; ++i) {
      futures.push_back(async(launch::async, &Game::render_slice, this, i));
    }
    for (auto &f : futures) {
      f.get();
    }
  }
}

void Game::run() {
  int i = 0;
  while (running) {
    handle_input();
    draw_game();
    scr->commit();
    if (i++ == 10) {
      cout << "FPS: " << scr->fps() << endl;
      i = 0;
    }
  }
}
