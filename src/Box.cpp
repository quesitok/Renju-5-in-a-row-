#include "Box.h"

Box::Box(SDL_Renderer* renderer, const int16_t pos, const int16_t col, const int16_t row)
    : renderer(renderer)
    , player(nullptr)
    , clicked(false)
    , state(false)
    , pos(pos)
    , row(row)
    , col(col)
{
    box = { 0, 0, 0, 0 };
}
Box::~Box() {
    if (player) {
        SDL_DestroyTexture(player);
    }
}

SDL_Color Box::getColor() { return color; }
bool Box::getClicked() { return clicked; }
bool Box::getState() { return state; }
int16_t Box::getPos() { return pos; }
void Box::loadPlayer(const bool state, std::string img_path) {
    clicked = true;
    this->state = state;
    std::string path = img_path + (state ? "black.png" : "white.png");
    player = loadTexture(path.c_str());
}
void Box::render() {
    SDL_Texture* currentTexture = clicked ? player : nullptr;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    renderLines();
    if (color.r || color.g || color.b) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawRect(renderer, &box);
    }
    if (currentTexture) {
        SDL_RenderCopy(renderer, currentTexture, nullptr, &playerBox);
    }
}
void Box::setClicked(const bool clicked) { this->clicked = clicked; }
void Box::setColor(const uint8_t r, const uint8_t g, const uint8_t b) { color = { r, g, b, 255 }; }
void Box::update(const int16_t x, const int16_t y) {
    box = { pos % col * x, pos / col * y, x, y };
    const int16_t ratio = 15;
    const int16_t padding = x / (2 * ratio);
    playerBox = { box.x + padding, box.y + padding, x * (ratio - 1) / ratio, y * (ratio - 1) / ratio };
}

SDL_Texture* Box::loadTexture(const char* img_path) {
    SDL_Surface* loadedSurface = IMG_Load(img_path);
    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    return newTexture;
}
void Box::renderLines() {
    const int16_t x = box.x + box.w / 2;
    const int16_t y = box.y + box.h / 2;
    // vertical lines
    if (pos / col == 0) {
        SDL_RenderDrawLine(renderer, x, y, x, y + box.h);
    } else if (pos / col == row - 1) {
        SDL_RenderDrawLine(renderer, x, box.y, x, y);
    } else {
        SDL_RenderDrawLine(renderer, x, box.y, x, box.y + box.h);
    }
    // horizontal lines
    if (pos % col == 0) {
        SDL_RenderDrawLine(renderer, x, y, box.x + box.w, y);
    } else if (pos % col == col - 1) {
        SDL_RenderDrawLine(renderer, box.x, y, x, y);
    } else {
        SDL_RenderDrawLine(renderer, box.x, y, box.x + box.w, y);
    }
}
