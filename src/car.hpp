#pragma once

#include <map>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include "agent.hpp"
#include "utilities.hpp"
#include <SFML/Graphics.hpp>

using Vector = std::vector<float>;
using Points = std::map<int, Vector>;

struct Car {

    float x;
    float y;
    int angle;
    int velocity;
    int max_speed;
    int max_cycles;
    float display_x;
    float display_y;
    sf::Image track;

    Points sensors = {
        {-45, {0, 0}},
        {0, {0, 0}},
        {45, {0, 0}}
    };

    Car(
        float disp_x,
        float disp_y,
        int cycles,
        int speed,
        sf::Image image
    ) {
        display_x = disp_x;
        display_y = disp_y;
        max_cycles = cycles;
        max_speed = speed;
        track = image;
    }

    static float distance(float x1, float x2, float y1, float y2) {
        return std::sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    bool off_track(float x, float y) {
        sf::Color color = track.getPixel(int(x), int(y));
        sf::Color crash = {0, 0, 0, 255};
        return color == crash;
    }


    Vector read_sensors() {
        // This method determines the distance from the car to
        // the wall along specified angles. These are the
        // car's sensors and its how it views the world.

        Vector state;

        for (auto& sensor : sensors) {

            float radians = float(angle + sensor.first) * M_PI / 180.0;
            float sensor_x = x;
            float sensor_y = y;

            for (int i = 0; i < max_cycles; i++) {
                sensor_x -= 1 * std::sin(radians);
                sensor_y += 1 * std::cos(radians);
                if (off_track(sensor_x, sensor_y)) {
                    break;
                }
            }

            sensor.second[0] = sensor_x;
            sensor.second[1] = sensor_y;

            state.emplace_back(distance(x, sensor_x, y, sensor_y));
        }

        return state;
    }

    Vector reset() {
        // This randomly searches the track and
        // resets the car to a valid position

        while (off_track(x, y)) {
            x = numerics::random() * display_x;
            y = numerics::random() * display_y;
            velocity = numerics::randint(1, 10);
            angle = numerics::randint(1, 359);
        }

        return read_sensors();
    }

    Vector step(int action, bool& done, float& reward) {
        // This is how the car takes a step
        // given an action. The Agent can pick
        // from turning left, turning right,
        // speeding up, or slowing down.
    
        Vector state = read_sensors();

        if (action == 0) {
            angle -= 5;
        } else if (action == 1) {
            angle += 5;
        } else if (action == 2) {
            velocity += 1;
        } else if (action == 3) {
            velocity -= 1;
        }

        angle = angle % 359;
        velocity = std::clamp(velocity, int(1), max_speed);

        float radians = float(angle) * M_PI / 180.0;
        float old_x = x;
        float old_y = y;

        x -= velocity * std::sin(radians);
        y += velocity * std::cos(radians);

        done = off_track(x, y);
        reward = distance(x, old_x, y, old_y);

        if (done) {
            reward = -1000;
        }

        return state;
    }
};
