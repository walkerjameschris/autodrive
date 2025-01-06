#pragma once

#include <map>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

using Vector = std::vector<float>;
using Points = std::map<int, Vector>;

struct Car {

    float x = 640;
    float y = 125;
    int angle = 270;
    int velocity = 0;

    float init_x = 640;
    float init_y = 125;
    int init_angle = 270;
    int init_velocity = 0;

    float display_x = 1280;
    float display_y = 720;

    int max_cycles = 200;

    Points sensors = {
        {-45, {0, 0}},
        {-15, {0, 0}},
        {0, {0, 0}},
        { 15, {0, 0}},
        { 45, {0, 0}}
    };

    static bool off_track(int x, int y, sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
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
        x = init_x;
        y = init_y;
        angle = init_angle;
        velocity = init_velocity;
        return read_sensors(image);
    }

    Vector step(
        int action,
        bool& done,
        float& reward,
        sf::Image& image
    ) {

        if (action == 1) {
            angle = (angle - 10) % 359;
        } else if (action == 2) {
            angle = (angle + 10) % 359;
        } else if (action == 3) {
            velocity = std::clamp(velocity + 1.0, 0.0, 10.0);
        } else if (action == 4) {
            velocity = std::clamp(velocity - 1.0, 0.0, 10.0);
        }

        float radians = float(angle) * M_PI / 180.0;

        x -= velocity * std::sin(radians);
        y += velocity * std::cos(radians);

        done = off_track(x, y, image);
        reward = 1.0;

        return read_sensors(image);
    }
};
