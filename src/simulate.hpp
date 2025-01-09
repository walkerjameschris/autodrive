#pragma once

#include <deque>
#include <string>
#include "car.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

namespace simulate {
    // This namespace contains helpers for training
    // and observing (rendering) an agent, enviornment
    // pairing. Generally, we train first and then
    // render the trained result.

    void train(int n, Car& car, Agent& agent, sf::Image& image) {

        // Epsilon (probability of random action) will decay
        // we also track the history of rewards, it should fall
        float epsilon = 1;
        std::deque<float> reward_history;

        // Train for a number of epochs
        for (int i = 0; i < n; i++) {

            // Reset the car to a random position on the track
            Vector state = car.reset(image);

            // These track the current epoch to update Q-values
            int cycle = 0;
            int action = 0;
            float reward = 0;
            float rewards = 0;
            bool done = false;
            float avg_reward = 0;

            // Train up to 1000 steps before resetting
            for (int j = 0; j < 1000; j++) {
                action = agent.get_action(state, epsilon);
                Vector next_state = car.step(action, done, reward, image);
                agent.update(state, next_state, action, reward);
                rewards = round(rewards + reward);
                state = next_state;
                if (done) {
                    break;
                }
            }

            // Decay epsilon and record rewards
            epsilon = std::max(float(0), epsilon - (1 / float(n)));
            reward_history.push_back(rewards);

            // Only keep track of 100 epochs at most
            if (reward_history.size() >= 100) {
                reward_history.pop_front();
            }

            // This computes mean reward for last 100 epochs
            for (float i : reward_history) {
                avg_reward += i / float(reward_history.size());
            }

            // Print report every 1000 epochs
            if ((i % 1000) == 0) {
                std::cout << " Epoch: " << std::to_string(i);
                std::cout << " Reward: " << std::to_string(avg_reward);
                std::cout << " Epsilon: " << std::to_string(epsilon);
                std::cout << std::endl;
            }
        }
    }

    void render(
        Car& car,
        Agent& agent,
        sf::Image& image,
        sf::Sprite& sprite,
        sf::RenderWindow& window
    ) {

        // This sets up a rendering cycle for one frame.
        // This is called once per frame in the SFML
        // render loop, so there is no need to iterate.
        // However, we do reset the car if it crashes.

        bool done;
        float reward;
        int action = agent.get_action(car.read_sensors(image), 0);
        Vector state = car.step(action, done, reward, image);

        // These represent the car and sensors
        sf::RectangleShape rectangle;
        sf::CircleShape circle;

        // Describe the car rectangle
        rectangle.setFillColor(sf::Color::Green);
        rectangle.setPosition({car.x, car.y});
        rectangle.setRotation(car.angle);
        rectangle.setOrigin({10, 20});
        rectangle.setSize({20, 30});

        // Describe the sensor circle
        circle.setFillColor(sf::Color::Red);
        circle.setOrigin({3, 3});
        circle.setRadius(3);

        // Draw the track
        window.clear();
        window.draw(sprite);

        // Draw all sensors
        for (auto& sensor : car.sensors) {
            float x = sensor.second[0];
            float y = sensor.second[1];
            circle.setPosition({x, y});
            window.draw(circle);
        }

        // Draw the car
        window.draw(rectangle);
        window.display();

        if (done) {
            car.reset(image);
        }
    }
}
