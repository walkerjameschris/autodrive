#pragma once

#include <SFML/Graphics.hpp>
#include "vehicle.hpp"

struct Simulation {

    Vehicle vehicle;

    void render(
        sf::RenderWindow& window,
        sf::Sprite& sprite,
        sf::Image& image,
        bool left,
        bool right,
        bool faster,
        bool slower
    ) {

        int action;

        if (left) {
            action = 1;
        } else if (right) {
            action = 2;
        } else if (faster) {
            action = 3;
        } else if (slower) {
            action = 4;
        } else {
            action = 0;
        }

        State sensors = vehicle.step(action, image);

        window.clear();
        window.draw(sprite);
        vehicle.render(window);
        window.display();
    }
};
