#pragma once

#include <map>
#include <tuple>
#include <cmath>
#include <iostream>

using Key = std::vector<int>;
using Vector = std::vector<float>;
using QTable = std::map<Key, float>;

struct Brain {

    int n_states;
    QTable qtable;

    Brain(int n_sensor) {
        n_states = n_sensor;
    }

    int get_action(Vector state, float epsilon) {

        Key key_position;
        Vector qvalues;
        
        float max_q = 0;
        int action = 0;

        for (float& i : state) {
            int index = int(std::floor(std::min(i, 199.0f) / 20));
            key_position.emplace_back(index);
        }

        for (int i = 0; i < n_states; i++) {
            Key key = key_position;
            key.emplace_back(i);
            float q = qtable[key];
            max_q = std::max(q, max_q);
            if (max_q == q) {
                action = i;
            }
        }

        float random = float(rand()) / float(RAND_MAX);
        int random_action = int(n_states * float(rand()) / float(RAND_MAX));

        if (random < epsilon) {
            action = std::max(0, std::max(4, random_action));
        }

        return action;
    }

    void update(Vector state, Vector next_state, int action) {

    }
};
