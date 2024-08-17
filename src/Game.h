#pragma once

#include "Renju.h"
#include <random>

class Game : Renju {
public:
    Game(std::string title, std::string img_path, const int16_t window_size, const int16_t num_of_columns, const int16_t num_of_rows, const int16_t target);
    ~Game();

    bool getRunning();
    void handleEvents();
    void init(const char* title);
    void render();
    void update();

private:
    SDL_Renderer* renderer;
    SDL_Window* window;
    bool running;
    int16_t x_side, y_side;
};
