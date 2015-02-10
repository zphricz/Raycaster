#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Screen.h"
#include "Vec2.h"

class Game {
    private:
        Screen* scr;
        int map_width;
        int map_height;
        float direction;
        Vec2 position;
        std::vector<int> map;
        std::vector<Color> colors;
        Color ceiling_color;
        Color floor_color;
        int num_threads;
        bool running;

        int& map_at(int x, int y);
        static void process_slice(Game& self, int i);
        void handle_input();
        void draw_game();
    public:
        Game(Screen* scr, const char* map_name);
        ~Game();
        void run();
};

#endif
