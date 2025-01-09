#pragma once

#include <map>
#include <vector>
#include <algorithm>

using Key = std::vector<int>;
using Vector = std::vector<float>;

namespace numerics {

    float random() {
        return float(rand()) / float(RAND_MAX);
    }

    int randint(int states = 3) {
        return std::floor(random() * (states + 1));
    }
};

struct QState {

    Vector q = {0, 0, 0, 0};

    float max() {
        return *std::max_element(q.begin(), q.end());
    }

    float argmax() {
        auto iterator = std::max_element(q.begin(), q.end());
        return std::distance(q.begin(), iterator);
    }
};

struct Agent {

    std::map<Key, QState> qtable;

    static int encode(float x, float divisor = 20) {
        return std::floor(x / divisor);
    }

    int get_action(Vector state, float epsilon) {

        if (numerics::random() < epsilon) {
            return numerics::randint();
        }

        Key key;

        for (float i : state) {
            key.emplace_back(encode(i));
        }

        return qtable[key].argmax();
    }

    void update(Vector state, Vector next_state, int action, float reward) {

        Key key;
        Key next_key;

        for (float i : state) {
            key.emplace_back(encode(i));
        }

        for (float i : next_state) {
            next_key.emplace_back(encode(i));
        }

        float q = qtable[key].q[action];
        float next_q = qtable[next_key].max();
        float solved_q = q + 0.05d * (reward + 0.9d * next_q - q);

        qtable[key].q[action] = solved_q;
    }
};
