#include <string>
#include <iostream>
#include "src/hud.hpp"
#include "src/car.hpp"
#include "src/agent.hpp"
#include "src/simulate.hpp"
#include <SFML/Graphics.hpp>

int main() {

    int fps = 60;
    int max_speed = 10;
    int epochs = 500000;
    int sensor_read = 200;
    float sensor_chunk = 20;
    float learn_rate = 0.05;
    float discount_rate = 0.90;

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Image image;

    texture.loadFromFile("../img/track.png");
    sprite.setTexture(texture);
    image = texture.copyToImage();

    sf::Vector2u display_dims = image.getSize();
    int display_x = display_dims.x;
    int display_y = display_dims.y;

    Car car(display_x, display_y, sensor_read, max_speed, image);
    Agent agent(sensor_chunk, learn_rate, discount_rate);
    HUD hud("../font/jetbrains.ttf");

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

        state = simulate::render(
            hud,
            car,
            agent,
            state,
            clock,
            sprite,
            window
        );
    }

    return 0;
}
