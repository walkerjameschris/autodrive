#include <SFML/Graphics.hpp>
#include "src/simulate.hpp"
#include "src/brain.hpp"
#include "src/car.hpp"
#include <iostream>
#include <string>

int main() {

    const int frame_rate = 60;
    const int display_x = 1280;
    const int display_y = 720;
    const int train_cycles = 10;

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Image image;

    texture.loadFromFile("../img/track.png");
    sprite.setTexture(texture);
    image = texture.copyToImage();

    Car car;
    Brain brain(car.sensors.size());

    Simulate::train(train_cycles, car, brain, image);

    sf::VideoMode video(display_x, display_y);
    sf::RenderWindow window(video, "autodrive");
    sf::Clock clock;
    sf::Event event;

    window.setFramerateLimit(frame_rate);
    
    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        Simulate::render(car, brain, image, sprite, window);
    }

    return 0;
}
