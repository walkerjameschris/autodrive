#pragma once

#include <string>
#include "car.hpp"
#include <iostream>
#include "utilities.hpp"
#include <SFML/Graphics.hpp>

namespace simulate {

    void train(int n, Car& car, Agent& agent) {
        // Function to train an agent for a specified number of
        // epochs. Throughout training we monitor loss in addition
        // to epsilon decay.

        float epsilon = 1;

        for (int i = 0; i < n; i++) {

            Vector state = car.reset();

            int cycle = 0;
            int action = 0;
            int rewards = 0;
            float reward = 0;
            bool done = false;

            for (int j = 0; j < 1000; j++) {
                action = agent.get_action(state, epsilon);
                Vector next_state = car.step(action, done, reward);
                agent.update(state, next_state, action, reward);
                rewards = round(rewards + reward);
                state = next_state;
                if (done) {
                    break;
                }
            }

            epsilon = std::max(float(0), epsilon - (2 / float(n)));

            if ((i % 1000) == 0) {
                std::cout << " Epoch: " << std::to_string(i);
                std::cout << " Reward: " << std::to_string(rewards);
                std::cout << " Epsilon: " << std::to_string(epsilon);
                std::cout << std::endl;
            }
        }
    }

    void render(
        HUD& hud,
        Car& car,
        Agent& agent,
        Vector& state,
        sf::Clock& clock,
        sf::Sprite& sprite,
        sf::RenderWindow& window,
        sf::Sprite& racecar_sprite
    ) {
        // This sets up a rendering cycle for one frame.
        // This is called once per frame in the SFML
        // render loop, so there is no need to iterate.
        // However, we do reset the car if it crashes.

        bool done;
        float reward;

        int action = agent.get_action(state, 0);
        state = car.step(action, done, reward);

        racecar_sprite.setPosition({car.x, car.y});
        racecar_sprite.setRotation(car.angle);
        racecar_sprite.setOrigin({10, 20});

        sf::CircleShape circle;

        circle.setFillColor(sf::Color::White);
        circle.setOrigin({4, 4});
        circle.setRadius(4);

        window.clear();
        window.draw(sprite);

        for (auto& sensor : car.sensors) {
            float x = sensor.second[0];
            float y = sensor.second[1];
            circle.setPosition({x, y});
            window.draw(circle);
        }

        hud.render(action, state, clock, window);
        window.draw(racecar_sprite);
        window.display();

        if (done) {
            car.reset();
        }
    }
}
