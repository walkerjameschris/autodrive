#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "car.hpp"
#include <string>

namespace Simulate {

    void train(
        int n,
        Car& car,
        Brain& brain,
        sf::Image& image
    ) {

        float epsilon = 1;

        for (int i = 0; i < n; i++) {

            Vector state = car.reset(image);

            int cycle = 0;
            int action = 0;
            float reward = 0;
            float rewards = 0;
            bool done = false;

            while (!done && cycle < 1000) {
                action = brain.get_action(state, epsilon);
                Vector next_state = car.step(action, done, reward, image);
                brain.update(state, next_state, action, reward);
                rewards = round(rewards + reward);
                state = next_state;
                cycle += 1;
            }

            epsilon = std::max(float(0), epsilon - (1 / float(n)));

            if ((i % 1000) == 0) {
                std::cout << " Epoch: " << std::to_string(i);
                std::cout << " Action: " << std::to_string(action);
                std::cout << " Epsilon: " << std::to_string(epsilon);
                std::cout << std::endl;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                break;
            }
        }
    }

    void render(
        Car& car,
        Brain& brain,
        sf::Image& image,
        sf::Sprite& sprite,
        sf::RenderWindow& window
    ) {

        bool done;
        float reward;
        int action = brain.get_action(car.read_sensors(image), 0);
        Vector state = car.step(action, done, reward, image);

        sf::RectangleShape rectangle;
        sf::CircleShape circle;

        rectangle.setFillColor(sf::Color::Green);
        rectangle.setPosition({car.x, car.y});
        rectangle.setRotation(car.angle);
        rectangle.setOrigin({10, 20});
        rectangle.setSize({20, 30});

        circle.setFillColor(sf::Color::Red);
        circle.setOrigin({3, 3});
        circle.setRadius(3);

        window.clear();
        window.draw(sprite);

        for (auto& sensor : car.sensors) {
            float x = sensor.second[0];
            float y = sensor.second[1];
            circle.setPosition({x, y});
            window.draw(circle);
        }

        window.draw(rectangle);
        window.display();

        if (done) {
            car.reset(image);
        }
    }
}
