#pragma once

#include <map>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include "agent.hpp"

using Vector = std::vector<float>;
using Points = std::map<int, Vector>;

struct Car {

    float x;
    float y;
    int angle;
    int velocity;
    int max_cycles;
    float display_x;
    float display_y;

    Points sensors = {
        {-45, {0, 0}},
        {0, {0, 0}},
        {45, {0, 0}}
    };

    Car(float disp_x, float disp_y, int cycles) {
        display_x = disp_x;
        display_y = disp_y;
        max_cycles = cycles;
    }

    static bool off_track(float x, float y, sf::Image& image) {
        sf::Color color = image.getPixel(int(x), int(y));
        sf::Color crash = {0, 0, 0, 255};
        return color == crash;
    }

    Vector read_sensors(sf::Image& image) {

        Vector state;

        for (auto& sensor : sensors) {

            float radians = float(angle + sensor.first) * M_PI / 180.0;
            float sensor_x = x;
            float sensor_y = y;
            int cycles = 0;

            while (!off_track(sensor_x, sensor_y, image) && cycles < max_cycles) {
                sensor_x -= 1 * std::sin(radians);
                sensor_y += 1 * std::cos(radians);
                cycles += 1;
            }

            sensor.second[0] = sensor_x;
            sensor.second[1] = sensor_y;

            float dist = std::sqrt(pow(x - sensor_x, 2) + pow(y - sensor_y, 2));
            state.emplace_back(dist);
        }

        return state;
    }

    Vector reset(sf::Image& image) {

        while (off_track(x, y, image)) {
            x = numerics::random() * display_x;
            y = numerics::random() * display_y;
            velocity = numerics::randint(9);
            angle = numerics::randint(359);
        }

        return read_sensors(image);
    }

    Vector step(
        int action,
        bool& done,
        float& reward,
        sf::Image& image
    ) {

        Vector state = read_sensors(image);

        if (action == 0) {
            angle = (angle - 5) % 359;
        } else if (action == 1) {
            angle = (angle + 5) % 359;
        } else if (action == 2) {
            velocity = std::clamp(velocity + 1.0, 1.0, 10.0);
        } else if (action == 3) {
            velocity = std::clamp(velocity - 1.0, 1.0, 10.0);
        }

        float radians = float(angle) * M_PI / 180.0;

        float old_x = x;
        float old_y = y;

        x -= velocity * std::sin(radians);
        y += velocity * std::cos(radians);

        done = off_track(x, y, image);
        reward = std::sqrt(pow(x - old_x, 2) + pow(y - old_y, 2));

        if (done) {
            reward = -1000;
        }

        return state;
    }
};
