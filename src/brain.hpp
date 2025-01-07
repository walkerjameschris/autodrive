#pragma once

#include <map>
#include <tuple>
#include <cmath>
#include <iostream>

using Vector = std::vector<float>;

namespace Numerics {
    float unif() {
        return float(rand()) / float(RAND_MAX);
    }

    int encode(float x, float low = 1, float high = 199) {
        return int(std::floor(std::clamp(x, low, high) / 25));
    }
}

struct QRecord {
    
    Vector values = {0, 0, 0, 0};

    int max() {

        float q = -INFINITY;

        for (float i : values) {
            q = std::max(q, i);
        }

        return q;
    }

    int argmax() {
        
        int index = 0;
        float q = -INFINITY;

        for (int i = 0; i < values.size(); i++) {
            if (values[i] > q) {
                index = i;
                q = values[i];
            }
        }

        return index;
    }
};

using Key = std::vector<int>;
using QTable = std::map<Key, QRecord>;

struct Brain {

    QTable qtable;

    int get_action(Vector state, float epsilon) {

        if (Numerics::unif() < epsilon) {
            float value = Numerics::unif() * 5;
            int action = std::floor(std::clamp(value, 0.01f, 4.99f));
            return action;
        }

        Key key;

        for (float i : state) {
            key.emplace_back(Numerics::encode(i));
        }

        return qtable[key].argmax();
    }

    void update(Vector state, Vector next_state, int action, float reward) {

        Key key;
        Key next_key;

        for (int i = 0; i < state.size(); i++) {
            key.emplace_back(Numerics::encode(state[i]));
            next_key.emplace_back(Numerics::encode(next_state[i]));
        }

        float q = qtable[key].values[action];
        float next_q = qtable[next_key].max();

        qtable[key].values[action] = q + 0.05d * (reward + 0.9d * next_q - q);
    }
};
