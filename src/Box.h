#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Box {
public:
    Box(SDL_Renderer* renderer, const int16_t pos, const int16_t col, const int16_t row);
    ~Box();

    SDL_Color getColor();
    bool getClicked();
    bool getState();
    int16_t getPos();
    void loadPlayer(const bool state, std::string img_path);
    void render();
    void setClicked(const bool clicked);
    void setColor(const uint8_t r, const uint8_t g, const uint8_t b);
    void update(const int16_t x, const int16_t y);

private:
    SDL_Texture* loadTexture(const char* img_path);
    void renderLines();

    SDL_Color color;
    SDL_Rect box;
    SDL_Rect playerBox;
    SDL_Renderer* renderer;
    SDL_Texture* player;
    bool clicked;
    bool state;
    const int16_t pos;
    const int16_t row, col;
};
