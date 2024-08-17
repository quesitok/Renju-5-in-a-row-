#pragma once

#include "Renju.h"
#include <chrono>
#include <map>
#include <random>

class Engine : Renju {
public:
    Engine(std::string img_path, const int16_t num_of_columns, const int16_t num_of_rows, const int16_t target, const bool decisiveResult, const bool state, std::vector<std::shared_ptr<Box>> grid, std::vector<int16_t> moves_played, std::vector<int16_t> moves_undone);
    ~Engine();

    int16_t getMove();

private:
    int16_t evaluatePos(const int16_t pos);

    const int16_t max_depth = 2;
    std::map<std::vector<std::shared_ptr<Box>>, std::vector<int16_t>> transposition_table;
};
