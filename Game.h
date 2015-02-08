#ifndef GAME_H
#define GAME_H

#include "Screen.h"
#include "Vec2.h"

enum { RED = 1, GREEN = 2, BLUE = 3};

class Game {
    private:
        Screen* scr;
        Vec2 position;
        float direction;
        bool running;

        static void process_slice(Game& self, int i);
        void handle_input();
        void draw_game();
    public:
        Game(Screen* scr);
        ~Game();
        void run();
};

#endif
