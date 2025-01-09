#pragma once

#include <map>
#include <vector>
#include <algorithm>

using Key = std::vector<int>;
using Vector = std::vector<float>;

namespace numerics {
    // A namespace of tools for generating randomness

    float random() {
        return float(rand()) / float(RAND_MAX);
    }

    int randint(int states = 3) {
        return std::floor(random() * (states + 1));
    }
};

struct QState {
    // This represents what action should be taken
    // given a current state. We store these records
    // in a hash-map within the agent and provide
    // methods for determine the best action and
    // its Q-values.

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
    // The brain of the car itself. Has data structures
    // for producing actions and updating its memories.

    float chunk_size;
    float learn_rate;
    float discount_rate;

    std::map<Key, QState> qtable;

    Agent(float chunk, float lr, float dr) {
        chunk_size = chunk;
        learn_rate = lr;
        discount_rate = dr;
    }

    int encode(float x) {
        return std::floor(x / chunk_size);
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
        float final_q = q + learn_rate * (reward + discount_rate * next_q - q);

        qtable[key].q[action] = final_q;
    }
};
