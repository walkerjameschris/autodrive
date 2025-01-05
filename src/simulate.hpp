#pragma once

#include <SFML/Graphics.hpp>
#include "brain.hpp"
#include <iostream>
#include "car.hpp"
#include <string>

namespace Simulate {

    void train(
        int n,
        Car& car,
        Brain& brain,
        sf::Image& image,
        int maximum_reward = 1000
    ) {

        for (int i = 0; i < n; i++) {

            float reward = 0;
            float rewards = 0;
            bool done = false;
            State state = car.reset(image);

            while (!done || rewards < maximum_reward) {
                int action = brain.get_action(state);
                State state = car.step(action, done, reward, image);
                brain.update(state, action);
                rewards = round(rewards + reward);
            }

            if ((i % 250) == 0) {
                std::cout << "Epoch: " << std::to_string(i) << ", ";
                std::cout << "Reward: " << std::to_string(rewards) << "\n";
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
        int action = brain.get_action(car.state);
        State state = car.step(action, done, reward, image);

        sf::RectangleShape rectangle;
        sf::CircleShape circle;

        rectangle.setPosition({car.x, car.y});
        rectangle.setRotation(car.angle);
        rectangle.setOrigin({30, 20});
        rectangle.setSize({60, 40});

        circle.setFillColor(sf::Color::Red);
        circle.setOrigin({5, 5});
        circle.setRadius(5);

        window.clear();
        window.draw(sprite);

        for (int i = 0; i < 3; i++) {
            float x = car.sensor_x_pos[i];
            float y = car.sensor_y_pos[i];
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
