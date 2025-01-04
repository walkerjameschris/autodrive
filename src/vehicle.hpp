#pragma once

#include <cmath>
#include <vector>
#include <algorithm>

using State = std::vector<float>;

struct Vehicle {

    float x = 640;
    float y = 100;
    int angle = 180;
    int velocity = 0;

    State state = {0, 0, 0};

    static bool off_track(int x, int y, sf::Image& image) {
        sf::Color color = image.getPixel(x, y);
        sf::Color crash = {0, 0, 0, 255};
        return color == crash;
    }

    State read_sensors() {
        return {0, 0, 0};
    }

    State step(int action, sf::Image& image) {

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

        x = std::clamp(x + velocity * std::cos(radians), float(0), float(1280));
        y = std::clamp(y - velocity * std::sin(radians), float(0), float(720));


        if (off_track(x, y, image)) {
            x = 640;
            y = 100;
            angle = 180;
            velocity = 0;
        }

        return read_sensors();
    }

    void render(sf::RenderWindow& window) {
        sf::CircleShape circle;
        circle.setPointCount(32);
        circle.setRadius(10);
        circle.setPosition({x, y});
        circle.setFillColor(sf::Color::Red);
        window.draw(circle);
    }
};
