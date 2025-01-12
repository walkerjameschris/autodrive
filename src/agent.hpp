#pragma once

#include <map>
#include <cmath>
#include <vector>
#include <algorithm>
#include "utilities.hpp"

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

    float lr;
    float dr;
    float chunk_size;

    std::map<Key, QState> qtable;

    Agent(float chunk, float learn_rate, float discount_rate) {
        chunk_size = chunk;
        dr = discount_rate;
        lr = learn_rate;
    }

    int encode(float x) {
        return std::floor(x / chunk_size);
    }

    int get_action(Vector state, float epsilon) {
 
        if (numerics::random() < epsilon) {
            return numerics::randint(0, 3);
        }

        Key key;

        for (float i : state) {
            key.emplace_back(encode(i));
        }

        return qtable[key].argmax();
    }

    void update(Vector state, Vector new_state, int action, float reward) {

        Key key;
        Key new_key;

        for (float i : state) {
            key.emplace_back(encode(i));
        }

        for (float i : new_state) {
            new_key.emplace_back(encode(i));
        }

        float q = qtable[key].q[action];
        float new_q = qtable[new_key].max();
        float final_q = q + lr * (reward + dr * new_q - q);

        qtable[key].q[action] = final_q;
    }
};
