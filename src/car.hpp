#pragma once

#include <map>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include "agent.hpp"
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

    Points sensors = {
        {-45, {0, 0}},
        {0, {0, 0}},
        {45, {0, 0}}
    };

    Car(float disp_x, float disp_y, int cycles, int speed) {
        display_x = disp_x;
        display_y = disp_y;
        max_cycles = cycles;
        max_speed = speed;
    }

    static bool off_track(float x, float y, sf::Image& image) {
        sf::Color color = image.getPixel(int(x), int(y));
        sf::Color crash = {0, 0, 0, 255};
        return color == crash;
    }

    Vector read_sensors(sf::Image& image) {

        // This method determines the distance from the car to
        // the wall along specified angles. These are the
        // car's sensors and its how it views the world.

        Vector state;

        // We iterate through all sensors and determine the
        // distance to the wall or stop early if a max viewing
        // distance is reached.
        for (auto& sensor : sensors) {

            float radians = float(angle + sensor.first) * M_PI / 180.0;
            float sensor_x = x;
            float sensor_y = y;

            for (int i = 0; i < max_cycles; i++) {
                sensor_x -= 1 * std::sin(radians);
                sensor_y += 1 * std::cos(radians);
                if (off_track(sensor_x, sensor_y, image)) {
                    break;
                }
            }

            sensor.second[0] = sensor_x;
            sensor.second[1] = sensor_y;

            float dist = std::sqrt(pow(x - sensor_x, 2) + pow(y - sensor_y, 2));
            state.emplace_back(dist);
        }

        return state;
    }

    Vector reset(sf::Image& image) {

        // This randomly searches the track and
        // resets the car to a valid position
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

        // This is how the car takes a step
        // given an action. The Agent can pick
        // from turning left, turning right,
        // speeding up, or slowing down.
    
        Vector state = read_sensors(image);

        // Translate action int to state changes
        if (action == 0) {
            angle -= 5;
        } else if (action == 1) {
            angle += 5;
        } else if (action == 2) {
            velocity += 1;
        } else if (action == 3) {
            velocity -= 1;
        }

        // Ensure car state is valid
        angle = angle % 359;
        velocity = std::clamp(velocity, int(1), max_speed);

        // Convert to radians
        float radians = float(angle) * M_PI / 180.0;
        float old_x = x;
        float old_y = y;

        // Move x and y positions
        x -= velocity * std::sin(radians);
        y += velocity * std::cos(radians);

        // Determine if crashed and reward (distance travelled)
        done = off_track(x, y, image);
        reward = std::sqrt(pow(x - old_x, 2) + pow(y - old_y, 2));

        // If crashed penalize heavily
        if (done) {
            reward = -1000;
        }

        return state;
    }
};
