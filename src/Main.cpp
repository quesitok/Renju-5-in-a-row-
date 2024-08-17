#include "Game.h"
#include <memory>
#include <string>

int main () {
    std::string title = "Renju";
    std::string img_path = "/home/pau/Pictures/Go_img/";
    const int16_t windowSize = 550;
    const int16_t col = 15;
    const int16_t row = col - 0;
    const int16_t target = 5;
    std::unique_ptr<Game> game = std::make_unique<Game>(title, img_path, windowSize, col, row, target);
    while (game->getRunning()) {
        game->handleEvents();
        game->update();
        game->render();
    }
}
