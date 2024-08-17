#include "Game.h"

Game::Game(std::string title, std::string img_path, const int16_t window_size, const int16_t num_of_columns, const int16_t num_of_rows, const int16_t target)
    : Renju(img_path, num_of_columns, num_of_rows, target)
    , x_side(window_size / num_of_columns)
    , y_side(window_size / num_of_rows)
{
    init(title.c_str());
}
Game::~Game() {  };

bool Game::getRunning() { return running; };
void Game::handleEvents() {
    SDL_Event event;
    if (!SDL_WaitEvent(&event)) {
	return;
    }
    int16_t index;
    int16_t x, y;
    switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                if (moves_played.size() == 2) {
                    break;
                }
                undoLast();
                state = !state;
                break;

            case SDLK_RIGHT:
                if (moves_undone.size() == 0) {
                    break;
                }
                grid.at(moves_undone.back())->setClicked(true);
                moves_played.push_back(moves_undone.back());
                if (isWinner(moves_undone.back())) {
                    decisiveResult = true;
                }
                moves_undone.pop_back();
                state = !state;
                break;

            case SDLK_UP:
                while (moves_played.size() > 2) {
                    undoLast();
                    state = !state;
                    update();
                    render();
                }
                break;

            case SDLK_DOWN:
                while (moves_undone.size() > 0) {
                    grid.at(moves_undone.back())->setClicked(true);
                    moves_played.push_back(moves_undone.back());
                    if (isWinner(moves_undone.back())) {
                        decisiveResult = true;
                    }
                    moves_undone.pop_back();
                    state = !state;
                    update();
                    render();
                }
                break;

            case SDLK_SPACE:
                int16_t life_time = 50;
                int16_t ran;
                bool complications;
                do {
                    auto random = rand() % (num_of_columns * num_of_rows);
                    ran = static_cast<int16_t>(random);
                    complications = processMove(ran) || applyRules(ran);
                    life_time--;
                } while (life_time && (complications));
                if (life_time) {
                    moves_undone.clear();
                    state = !state;
                }

                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            x = event.button.x / x_side;
            y = event.button.y / y_side;
            index = y * num_of_columns + x;
            if (x >= num_of_columns || y >= num_of_rows || processMove(index) || applyRules(index)) {
                break;
            }
            moves_undone.clear();
            state = !state;
            break;

        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                x_side = std::min(event.window.data1 / num_of_columns, event.window.data2 / num_of_rows);
                y_side = std::min(event.window.data1 / num_of_columns, event.window.data2 / num_of_rows);
            }
            for (auto& box : grid) {
                box->update(x_side, y_side);
            }
            break;
        }
}
void Game::init(const char* title) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        throw SDL_GetError();
    }
    x_side = std::min(x_side, y_side);
    y_side = std::min(x_side, y_side);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, x_side * num_of_columns, y_side * num_of_rows, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer || !window) {
        throw SDL_GetError();
    }

    srand(time(NULL));
    running = true;
    decisiveResult = false;
    state = true;
    // create board
    for (int16_t i = 0; i < num_of_columns * num_of_rows; i++) {
        grid.push_back(std::make_shared<Box>(renderer, i, num_of_columns, num_of_rows));
        grid.at(i)->update(x_side, y_side);
    }
    processMove(num_of_rows / 2 * num_of_columns + num_of_columns / 2);
    state = !state;

    processMove((num_of_rows / 2 + 1) * num_of_columns + (num_of_columns / 2 + 1));
    state = !state;
}
void Game::render() {
    SDL_SetRenderDrawColor(renderer, 190, 151, 91, 255);
    SDL_RenderClear(renderer);
    for (auto& box : grid) {
        box->render();
    }
    SDL_RenderPresent(renderer);
}
void Game::update() {
    for (auto& box : grid) {
        if (decisiveResult && box->getClicked() && box->getState() != state) {
            box->setColor(0, 0, 255); // show winning player
        } else if (box->getColor().r || box->getColor().g || box->getColor().b) {
            box->setColor(0, 0, 0); // hide non last moves
        }
    }
    grid.at(moves_played.back())->setColor(255, 0, 0); // show last move
}

