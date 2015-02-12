#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Screen.h"
#include "Vec.h"

class Game {
    private:
        Screen* scr;
        int map_width;
        int map_height;
        float direction;
        float height;
        float pitch;
        float fov;
        float plane_width;
        float plane_distance;
        Vec2f position;
        std::vector<char> map;
        std::vector<Color> colors;
        Color ceiling_color;
        Color floor_color;
        const int num_threads;
        bool running;

        char& map_at(int x, int y);
        void render_slice(int i);
        void handle_input();
        void draw_game();
    public:
        Game(Screen* scr, const char* map_name, int num_threads);
        ~Game();
        void run();
};

#endif
