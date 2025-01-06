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
        sf::Image& image
    ) {

        float epsilon = 1;

        for (int i = 0; i < n; i++) {

            float reward = 0;
            float rewards = 0;
            bool done = false;
            Vector state = car.reset(image);
            int cycle = 0;
        

            while (!done && cycle < 1000) {
                int action = brain.get_action(state, epsilon);
                Vector next_state = car.step(action, done, reward, image);
                brain.update(state, next_state, action);
                rewards = round(rewards + reward);
                state = next_state;
                cycle += 1;
            }

            epsilon = std::max(float(0), epsilon - (2 / float(n)));

            if ((i % 250) == 0) {
                std::cout << "Epoch: " << std::to_string(i) << " ";
                std::cout << "Reward: " << std::to_string(rewards) << std::endl;
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
        int action = brain.get_action(car.read_sensors(image), 1);
        Vector state = car.step(action, done, reward, image);

        sf::RectangleShape rectangle;
        sf::CircleShape circle;

        rectangle.setPosition({car.x, car.y});
        rectangle.setRotation(car.angle);
        rectangle.setOrigin({20, 30});
        rectangle.setSize({40, 60});

        circle.setFillColor(sf::Color::Red);
        circle.setOrigin({5, 5});
        circle.setRadius(5);

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
