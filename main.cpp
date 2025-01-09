#include <string>
#include <iostream>
#include "src/car.hpp"
#include "src/agent.hpp"
#include "src/simulate.hpp"
#include <SFML/Graphics.hpp>

int main() {

    // Hyperparameters
    const int fps = 60;
    const int max_speed = 10;
    const int epochs = 500000;
    const int sensor_read = 200;
    const float sensor_chunk = 20;
    const float learn_rate = 0.05;
    const float discount_rate = 0.90;

    // Containers for track image
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Image image;

    // Load track image
    texture.loadFromFile("../img/track.png");
    sprite.setTexture(texture);
    image = texture.copyToImage();

    // Record image width and height, ideally 1280 x 720
    sf::Vector2u display_dims = image.getSize();
    const int display_x = display_dims.x;
    const int display_y = display_dims.y;

    // Create car and agent then train
    Car car(display_x, display_y, sensor_read, max_speed);
    Agent agent(sensor_chunk, learn_rate, discount_rate);
    simulate::train(epochs, car, agent, image);

    // Launch SFML window
    sf::VideoMode video(display_x, display_y);
    sf::RenderWindow window(video, "autodrive");
    window.setFramerateLimit(fps);
    sf::Clock clock;
    sf::Event event;

    // Start rendering loop for SFML
    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulate::render(car, agent, image, sprite, window);
    }

    return 0;
}
