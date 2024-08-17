#pragma once

#include "Box.h"
#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

struct Counter {
    Counter(int16_t a, int16_t b) : cnt_1(a), cnt_2(b), b_1(-1), b_2(-1) {  };
    int16_t cnt_1, cnt_2;
    int16_t b_1, b_2;
};

class Renju {
public:
    Renju(std::string img_path, const int16_t num_of_columns, const int16_t num_of_rows, const int16_t target);
    ~Renju();

protected:
    bool isWinner(const int16_t pos);
    bool straight4();
    bool over5();
    std::array<std::shared_ptr<Counter>, 4> countAll(const int16_t pos);
    std::shared_ptr<Counter> countDir(const int16_t pos, int16_t step, std::function<bool(int)> additionalCheck = [](int) { return true; });
    std::shared_ptr<Counter> countD1(const int16_t pos);
    std::shared_ptr<Counter> countD2(const int16_t pos);
    std::shared_ptr<Counter> countH(const int16_t pos);
    std::shared_ptr<Counter> countV(const int16_t pos);
    int16_t fourCount();
    int16_t threeCount(const int16_t pos, const int16_t depth);
    int16_t twoCount(const int16_t pos, const int16_t depth);
    int16_t applyRules(const int16_t pos);
    int16_t processMove(const int16_t pos);
    void undoLast();

    bool decisiveResult;
    bool state;
    const int16_t num_of_columns;
    const int16_t num_of_rows;
    const int16_t target;
    const std::string img_path;
    int16_t num_of_four;
    int16_t num_of_three;
    int16_t num_of_two;
    std::array<std::shared_ptr<Counter>, 4> count_all;
    std::vector<int16_t> moves_played;
    std::vector<int16_t> moves_undone;
    std::vector<std::shared_ptr<Box>> grid;
};
