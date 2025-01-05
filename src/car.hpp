#pragma once

#include <cmath>
#include <vector>
#include <algorithm>

using State = std::vector<float>;

struct Car {

    float x = 640;
    float y = 125;
    int angle = 180;
    int velocity = 0;

    float init_x = 640;
    float init_y = 125;
    int init_angle = 180;
    int init_velocity = 0;

    float display_x = 1280;
    float display_y = 720;

    State state = {0, 0, 0};
    State sensor_x_pos = {0, 0, 0};
    State sensor_y_pos = {0, 0, 0};

    static bool off_track(int x, int y, sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        sf::Color crash = {0, 0, 0, 255};
        return color == crash;
    }

    State read_sensors(sf::Image& image) {

        State state = {0, 0, 0};
        State angles = {-45, 0, 45};

        for (int i = 0; i < 3; i++) {

            float radians = angles[i] * M_PI / 180.0;
            float sensor_x = x;
            float sensor_y = y;
            int cycles = 0;

            while (!off_track(sensor_x, sensor_y, image) && cycles < 10) {
                sensor_x += 1 * std::cos(radians);
                sensor_y -= 1 * std::sin(radians);
            }

            sensor_x_pos[i] = sensor_x;
            sensor_y_pos[i] = sensor_y;
        }

        return state;
    }

    State reset(sf::Image& image) {
        x = init_x;
        y = init_y;
        angle = init_angle;
        velocity = init_velocity;
        return read_sensors(image);
    }

    State step(
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

        x -= velocity * std::cos(radians);
        y += velocity * std::sin(radians);

        done = off_track(x, y, image);
        reward = 1.0;

        return read_sensors(image);
    }
};
