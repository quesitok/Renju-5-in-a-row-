#include "Renju.h"

Renju::Renju(std::string img_path, const int16_t num_of_columns, const int16_t num_of_rows, const int16_t target)
    : num_of_columns(num_of_columns)
    , num_of_rows(num_of_rows)
    , target(target)
    , img_path(img_path)
{  }
Renju::~Renju() {  }

std::array<std::shared_ptr<Counter>, 4> Renju::countAll(const int16_t pos) {
    std::array<std::shared_ptr<Counter>, 4> count;
    count.at(0) = countD1(pos);
    count.at(1) = countD2(pos);
    count.at(2) = countH(pos);
    count.at(3) = countV(pos);

    return count;
}
std::shared_ptr<Counter> Renju::countDir(const int16_t pos, int16_t step, std::function<bool(int)> additionalCheck) {
    std::shared_ptr<Counter> count = std::make_shared<Counter>(0, 0);
    for (int16_t i = 1; i < std::max(num_of_columns, num_of_rows); i++) {
        int16_t newPos = pos + i * step;
	if (newPos < 0 || newPos >= num_of_columns * num_of_rows || !additionalCheck(newPos)) {
	    break;
	}
	if (!grid.at(newPos)->getClicked()) {
	    if (count->b_1 >= 0) {
		break;
	    }
	    count->b_1 = newPos;
	} else if (grid.at(newPos)->getState() == state) {
	    count->cnt_1++;
	    if (count->b_1 == -1) {
		count->cnt_2++;
	    }
	} else {
	    break;
	}
        
    }

    return count;
}
std::shared_ptr<Counter> Renju::countD1(const int16_t pos) {
    int16_t step = num_of_columns + 1;
    std::shared_ptr<Counter> count = std::make_shared<Counter>(1, 1);
    std::shared_ptr<Counter> count_1 = countDir(pos, -step, [this, pos](int16_t p) { return p % num_of_columns < pos % num_of_columns && p / num_of_columns < pos / num_of_columns; }); // up-left
    count->b_1 = count_1->b_1;
    std::shared_ptr<Counter> count_2 = countDir(pos, step, [this, pos](int16_t p) { return p % num_of_columns > pos % num_of_columns && p / num_of_columns > pos / num_of_columns; }); // down-right
    count->b_2 = count_2->b_1;
    // final sum
    count->cnt_1 = count->cnt_1 + count_1->cnt_2 + count_2->cnt_2 + std::max(count_1->cnt_1 - count_1->cnt_2, count_2->cnt_1 - count_2->cnt_2);
    count->cnt_2 = count->cnt_2 + count_1->cnt_2 + count_2->cnt_2;

    return count;
}
std::shared_ptr<Counter> Renju::countD2(const int16_t pos) {
    int16_t step = num_of_columns - 1;
    std::shared_ptr<Counter> count = std::make_shared<Counter>(1, 1);
    std::shared_ptr<Counter> count_1 = countDir(pos, -step, [this, pos](int16_t p) { return p % num_of_columns > pos % num_of_columns && p / num_of_columns < pos / num_of_columns; }); // up-right
    count->b_1 = count_1->b_1;
    std::shared_ptr<Counter> count_2 = countDir(pos, step, [this, pos](int16_t p) { return p % num_of_columns < pos % num_of_columns && p / num_of_columns > pos / num_of_columns; }); // down-left
    count->b_2 = count_2->b_1;
    // final sum
    count->cnt_1 = count->cnt_1 + count_1->cnt_2 + count_2->cnt_2 + std::max(count_1->cnt_1 - count_1->cnt_2, count_2->cnt_1 - count_2->cnt_2);
    count->cnt_2 = count->cnt_2 + count_1->cnt_2 + count_2->cnt_2;

    return count;
}
std::shared_ptr<Counter> Renju::countH(const int16_t pos) {
    int16_t step = 1;
    std::shared_ptr<Counter> count = std::make_shared<Counter>(1, 1);
    std::shared_ptr<Counter> count_1 = countDir(pos, -step, [this, pos](int16_t p) { return p / num_of_columns == pos / num_of_columns; }); // left
    count->b_1 = count_1->b_1;
    std::shared_ptr<Counter> count_2 = countDir(pos, step, [this, pos](int16_t p) { return p / num_of_columns == pos / num_of_columns; }); // right
    count->b_2 = count_2->b_1;
    // final sum
    count->cnt_1 = count->cnt_1 + count_1->cnt_2 + count_2->cnt_2 + std::max(count_1->cnt_1 - count_1->cnt_2, count_2->cnt_1 - count_2->cnt_2);
    count->cnt_2 = count->cnt_2 + count_1->cnt_2 + count_2->cnt_2;

    return count;
}
std::shared_ptr<Counter> Renju::countV(const int16_t pos) {
    int16_t step = num_of_columns;
    std::shared_ptr<Counter> count = std::make_shared<Counter>(1, 1);
    std::shared_ptr<Counter> count_1 = countDir(pos, -step, [this, pos](int16_t p) { return p % num_of_columns == pos % num_of_columns; }); // left
    count->b_1 = count_1->b_1;
    std::shared_ptr<Counter> count_2 = countDir(pos, step, [this, pos](int16_t p) { return p % num_of_columns == pos % num_of_columns; }); // right
    count->b_2 = count_2->b_1;
    // final sum
    count->cnt_1 = count->cnt_1 + count_1->cnt_2 + count_2->cnt_2 + std::max(count_1->cnt_1 - count_1->cnt_2, count_2->cnt_1 - count_2->cnt_2);
    count->cnt_2 = count->cnt_2 + count_1->cnt_2 + count_2->cnt_2;

    return count;
}
bool Renju::isWinner(const int16_t pos) {
    auto count_now = countAll(pos);
    for (auto it = count_now.begin(); it != count_now.end(); it++) {
        auto count = *it;
        if (count->cnt_2 == target) {
            return true;
        }
    }

    return state ? false : over5();
}
bool Renju::straight4() {
    for (auto it = count_all.begin(); it != count_all.end(); it++) {
        auto count = *it;
        if (count->cnt_2 == 4 && count->b_1 >= 0 && count->b_2 >= 0) {
            if (isWinner(count->b_1) && isWinner(count->b_2)) {
                return true;
            }
        }
    }

    return false;
}
bool Renju::over5() {
    for (auto it = count_all.begin(); it != count_all.end(); it++) {
        auto count = *it;
        if (count->cnt_2 > target) {
            return true;
        }
    }

    return false;
}
int16_t Renju::fourCount() {
    int16_t cnt = 0;
    for (auto it = count_all.begin(); it != count_all.end(); it++) {
        auto count = *it;
        bool four = false;
        if (count->cnt_1 >=4 && count->b_1 >= 0 && isWinner(count->b_1)) {
            four = true;
        }
        if (count->cnt_1 >= 4 && count->b_2 >= 0 && isWinner(count->b_2)) {
            four = true;
        }
        if (four) {
            cnt++;
        }
    }

    return cnt;
}
int16_t Renju::threeCount(const int16_t pos, const int16_t depth) {
    if (depth == 0) {
        return 0;
    }
    int16_t cnt = 0;
    auto count_now = countAll(pos);
    for (auto it = count_now.begin(); it != count_now.end(); it++) {
        auto count = *it;
        bool three = false;
        if (count->cnt_1 >= 3 && count->b_1 >= 0 && count->b_2 >= 0) {
            processMove(count->b_1);
            if (straight4()) {
                if (!state) {
                    three = true;
                } else if (!over5() && fourCount() < 2 && threeCount(count->b_1, depth - 1) < 2) {
                    three = true;
                }
            }
            undoLast();
            moves_undone.pop_back();

            processMove(count->b_2);
            if (straight4()) {
                if (!state) {
                    three = true;
                } else if (!over5() && fourCount() < 2 && threeCount(count->b_2, depth - 1) < 2) {
                    three = true;
                }
            }
            undoLast();
            moves_undone.pop_back();
            if(three) {
                cnt++;
            }
        }
    }

    return cnt;
}
int16_t Renju::twoCount(const int16_t pos, const int16_t depth) {
    int16_t cnt = 0;
    auto count_now = countAll(pos);
    for (auto it = count_now.begin(); it != count_now.end(); it++) {
        auto count = *it;
        bool two = false;
        if (count->cnt_1 >= 2 && count->b_1 >= 0 && count->b_2 >= 0) {
            processMove(count->b_1);
            if (threeCount(count->b_1, depth)) {
                two = true;
            }
            undoLast();
            moves_undone.pop_back();

            processMove(count->b_2);
            if (threeCount(count->b_2, depth)) {
                two = true;
            }
            undoLast();
            moves_undone.pop_back();
            if(two) {
                cnt++;
            }
        }
    }

    return cnt;
}
int16_t Renju::applyRules(const int16_t pos) {
    decisiveResult = isWinner(pos);
    if (decisiveResult) {
        return 0;
    }
    bool over = over5();
    num_of_four = fourCount();
    num_of_three = threeCount(pos, 2);
    if (state && (over || num_of_four >= 2 || num_of_three >= 2)) {
        undoLast();
        moves_undone.pop_back();
        return 1;
    }
    return 0;
}
int16_t Renju::processMove(const int16_t pos) {
    if (decisiveResult || grid.at(pos)->getClicked()) {
        return 1;
    }
    grid.at(pos)->loadPlayer(state, img_path);
    moves_played.push_back(pos);
    count_all = countAll(pos);
    return 0;
}
void Renju::undoLast() {
    if (moves_played.size() > 2) {
        decisiveResult = false;
        grid.at(moves_played.back())->setClicked(false);
        moves_undone.push_back(moves_played.back());
        moves_played.pop_back();
    }
}

