#include <string>
#include <iostream>
#include "src/car.hpp"
#include "src/agent.hpp"
#include "src/simulate.hpp"
#include "src/utilities.hpp"
#include <SFML/Graphics.hpp>

int main() {

    int fps = 60;
    int max_speed = 10;
    int epochs = 100000;
    int display_y = 720;
    int display_x = 1280;
    int sensor_read = 200;
    float sensor_chunk = 15;
    float learn_rate = 0.05;
    float discount_rate = 0.90;

    HUD hud("../font/jetbrains.ttf");

    Element track("../img/track.png");
    Element racecar("../img/car.png");

    Agent agent(sensor_chunk, learn_rate, discount_rate);
    Car car(display_x, display_y, sensor_read, max_speed, track.image);

    simulate::train(epochs, car, agent);

    sf::VideoMode video(display_x, display_y);
    sf::RenderWindow window(video, "autodrive");
    window.setFramerateLimit(fps);
    sf::Clock clock;
    sf::Event event;

    Vector state = car.reset();

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulate::render(
            hud, car, agent, state, clock,
            track.sprite, window, racecar.sprite
        );
    }

    return 0;
}
