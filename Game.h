#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Screen.h"
#include "Vec.h"

class Game {
private:
  SoftScreen *scr;
  int map_width;
  int map_height;
  float direction;
  float height;
  float pitch;
  float fov;
  const float plane_width;
  const float plane_height;
  float plane_distance;
  Linear::Vec2f position;
  std::vector<char> map;
  std::vector<SDL_Color> colors;
  SDL_Color ceiling_color;
  SDL_Color floor_color;
  const int num_threads;
  bool running;

  char &map_at(int x, int y);
  void render_slice(int i);
  void handle_input();
  void draw_game();

public:
  Game(SoftScreen *scr, const char *map_name, int num_threads);
  ~Game();
  void run();
};

#endif
