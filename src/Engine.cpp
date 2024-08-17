#include "Engine.h"
#include <iostream>

Engine::Engine(std::string img_path, const int16_t num_of_columns, const int16_t num_of_rows, const int16_t target, const bool decisiveResult, const bool state, std::vector<std::shared_ptr<Box>> grid, std::vector<int16_t> moves_played, std::vector<int16_t> moves_undone)
    : Renju(img_path, num_of_columns, num_of_rows, target)
{  }
Engine::~Engine() {  }

int16_t Engine::getMove() {
    std::cout << std::endl;
    int16_t best = 0;
    int16_t move = rand() % (num_of_columns * num_of_rows);
    int16_t size = num_of_columns * num_of_rows;
        int16_t ran = rand() % (size / 2);
        ran = rand() % 2 ? size / 2 + ran : size / 2 - ran;
        return ran;
    auto start_time = std::chrono::system_clock::now();
    auto check_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(check_time - start_time).count();
    do {
        check_time = std::chrono::system_clock::now();
        duration = std::chrono::duration_cast<std::chrono::seconds>(check_time - start_time).count();
        int16_t ran = rand() % (size / 2);
        ran = rand() % 2 ? size / 2 + ran : size / 2 - ran;
        std::cout << ran;
        if (processMove(ran) || applyRules(ran)) {
            continue;
        }
        int16_t eval = evaluatePos(ran);
        undoLast();
        if (eval == 1000) {
            return ran;
        } else if (eval > best) {
            best = eval;
            move = ran;
        }
        std::cout << "->" << eval << std::endl;
    } while (duration < 5);
    std::cout << move << std::endl;
    return move;
}

int16_t Engine::evaluatePos(const int16_t pos) {
    if (isWinner(pos)) {
        return 1000;
    }
    int32_t eval = 0;

    int16_t four_single_weight = 85;
    int16_t three_single_weight = 45;
    int16_t two_single_weight = 15;
    for (auto it = count_all.begin(); it != count_all.end(); it++) {
        auto count = *it;
        switch (count->cnt_1) {
        case 2:
            eval += two_single_weight;
            break;

        case 3:
            eval += three_single_weight;
            break;

        case 4:
            eval += four_single_weight;
            break;
        }
    }
    int16_t min = 2;
    int16_t four_weight = 250;
    int16_t three_weight = 175;
    int16_t two_weight = 75;

    eval += min * four_weight * straight4();
    eval += four_weight * num_of_four;
    eval += three_weight * num_of_three;
    eval += two_weight * num_of_two;

    return eval;
}
