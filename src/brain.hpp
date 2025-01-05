#pragma once

#include <map>
#include <cmath>

using State = std::vector<float>;

struct Brain {

    int get_action(State state) {
        int min = 1;
        int max = 10;
        int random_num = min + rand() % (max - min);
        return random_num;
    }

    void update(State state, int action) {

    }
};
